#include "webScraper.h"

#include <string>

#include "util.h"

class Wrapper {
public:
	Wrapper() {};
	int setup(bool secured, SSL_CTX* ctx, string connection) {
		this->connection = connection;

		if (secured) {
			bio = BIO_new_ssl_connect(ctx);
		}
		else {
			bio = BIO_new_connect(getCon());
		}

		if (bio == nullptr)
		{
			R_ERROR("Failed to create BIO object for '%s'", getCon());
			return WebScraper::Error::CONNECTION_ERROR;
		}

		if (secured) {
			BIO_get_ssl(bio, &ssl);
			SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
			BIO_set_conn_hostname(bio, getCon());
		}

		//check connection
		if (BIO_do_connect(bio) <= 0) {
			R_ERROR("Durning connection check to '%s'", getCon());
			return WebScraper::Error::CONNECTION_ERROR;
		}

		//certificate check
		if (secured) {
			//certificate handshake
			if (BIO_do_handshake(bio) != 1) {
				R_ERROR("Error establishing SSL connection to: '%s'", getCon());
				return WebScraper::Error::SSL_ERROR;
			}

			//verify certificate
			if (SSL_get_verify_result(ssl) != X509_V_OK) {
				R_ERROR("Error cetificate verification failed for: '%s'", getCon());
				return WebScraper::Error::SSL_ERROR;
			}
		}

		return WebScraper::Error::OK;
	}

	BIO* getBio() {
		return bio;
	}

	SSL* getSsl() {
		return ssl;
	}

	const char* getCon() {
		return connection.data();
	}

	~Wrapper() {
		BIO_free_all(bio);
	}
private:
	string connection;

	SSL* ssl;
	BIO* bio;
};

WebScraper::WebScraper(string_view certAddr, string_view certFile) {
	if (certAddr.empty() && certFile.empty()) {
		ctx = SSL_CTX_new(SSLv23_client_method());
		if(!ctx)
		{
			R_ERROR("Error: SSL_CTX_new failed");
			exit(EXIT_FAILURE);
		}
		
		if (SSL_CTX_set_default_verify_paths(ctx) != 1) {
			R_ERROR("Error: SSL_CTX_set_default_verify_paths failed");
			exit(EXIT_FAILURE);
		}
	}
	if (!certAddr.empty()) {
		if (SSL_CTX_load_verify_locations(ctx, nullptr, certAddr.data()) != 1) {
			R_ERROR("Error loading certification folder");
			exit(EXIT_FAILURE);
		}
	}
	if (!certFile.empty()) {
		if (SSL_CTX_load_verify_locations(ctx, nullptr, certFile.data()) != 1) {
			R_ERROR("Error loading certification file");
			exit(EXIT_FAILURE);
		}
	}
};

int WebScraper::run(const ParsedLink &link, const size_t tries) {
	int return_code;
	for (uint i = 0; i < tries; i++) {
		//run web scrapper
		return_code = _run(link);
		//exit from loop when done
		if (return_code == Error::OK)break;
		//check for critical errors
		if (return_code > Error::CRITICAL_ERROR)break;
		//clear response
		response = PasedResponse();
	}
	return return_code;
}

int WebScraper::_run(ParsedLink link) {	
	bool secured = link.protocol == "https://";
	link.port = link.port == "" ? (secured ? ":443" : ":80") : link.port;

	Wrapper wrap;
	R_PRINT("Connecting to '%s'", link.host.data());
	int return_code = wrap.setup(secured, ctx, {link.host + link.port});
	if (return_code != 0) {
		return return_code;
	}

	R_PRINT("Connected and sending GET request");
	//send request
	string request{};
	request += "GET " + link.path + link.query + " HTTP/1.1\r\n";
	request += "Host: " + link.host + "\r\n";
	request += "\r\n";
	if (BIO_write(wrap.getBio(), request.data(), request.length()) < 0) {
		if (BIO_should_retry(wrap.getBio())) {
			R_ERROR("While sending request to: '%s'", wrap.getCon());
			return Error::SEND_RETRY_ERROR;
		}else {
			R_ERROR("Failed to send request to '%s'", wrap.getCon());
			return Error::SEND_ERROR;
		}
	}
	
	R_PRINT("GET request sent, waiting for response");
	//receive response
	const uint max_len = 1024;
	int len = max_len;
	char buffer[max_len];
	buffer[max_len] = '\0';
	//get data lenght from header when aviable
	bool headerLoaded = false;
	size_t maxDataLenght = -1;
	//recive data until time out or end of data
	while ((len = BIO_read(wrap.getBio(), buffer, 1024)) > 0) {
		response.append(buffer, len);
		if (!headerLoaded) {
			if (response.isHeaderValid()) {
				if (int r = response.getCode() != 200) {
					R_ERROR("Bad response [%d] from % s", r, wrap.getCon());
					return Error::BAD_RESPONSE;
				}else {
					R_PRINT("Response [%d] from %s", r, wrap.getCon());
				}
				
				headerLoaded = true;
				maxDataLenght = response.headerDataLenght();
			}
		}

		if (maxDataLenght == response.loadedDataLenght()) {
			//all data has been loaded
			break;
		}
	}

	//check recived data
	if (len <= 0){
		if (len == 0) {
			R_ERROR("Connection to '%s' closed", wrap.getCon());
			return Error::CONNECTION_CLOSED;
		}else if(BIO_should_retry(wrap.getBio())) {
			R_ERROR("Something went wrong while reciving data from '%s'", wrap.getCon());
			return Error::RECEIVE_RETRY_ERROR;
		}else {
			R_ERROR("Failed to recive data from '%s'", wrap.getCon());
			return Error::RECEIVE_ERROR;
		}
	}
	
	R_PRINT("Response recived");
	D_PRINT("Header valid:      %s", B(response.isHeaderValid()));
	D_PRINT("Loaded data lenght:%5lu/%lu", response.loadedDataLenght(), response.headerDataLenght());
	
	return Error::OK;
}