#include "webScraper.h"

#include <string>

#include "util.h"

//wrapper class for BIO and SSL connection
class Wrapper {
public:
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
			Logger::Print(RError, "Failed to create BIO object for '%s'", getCon());
			return WebScraper::Error::CONNECTION_ERROR;
		}

		if (secured) {
			BIO_get_ssl(bio, &ssl);
			if (ssl == NULL) {
				Logger::Print(RError, "Can't locate ssl pointer");
				return WebScraper::Error::SSL_ERROR;
			}
			SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
			BIO_set_conn_hostname(bio, getCon());
		}

		//check connection
		if (BIO_do_connect(bio) <= 0) {
			Logger::Print(RError, "During connection to '%s'", getCon());
			return WebScraper::Error::CONNECTION_ERROR;
		}

		//certificate check
		if (secured) {
			//certificate handshake
			if (BIO_do_handshake(bio) != 1) {
				Logger::Print(RError, "Error establishing SSL connection to: '%s'", getCon());
				return WebScraper::Error::SSL_ERROR;
			}

			//verify certificate
			if (SSL_get_verify_result(ssl) != X509_V_OK) {
				Logger::Print(RError, "Error certificate verification failed for: '%s'", getCon());
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
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (!ctx)
	{
		Logger::Print(RError, "SSL_CTX_new failed");
		exit(EXIT_FAILURE);
	}

	//load certificates
	if (certAddr.empty() && certFile.empty()) {

		if (SSL_CTX_set_default_verify_paths(ctx) != 1) {
			Logger::Print(RError, "SSL_CTX_set_default_verify_paths failed");
			exit(EXIT_FAILURE);
		}
	}
	else {
		const char* file = certFile.size() > 0 ? certFile.data() : nullptr;
		const char* adr = certAddr.size() > 0 ? certAddr.data() : nullptr;
		if (file != nullptr || adr != nullptr) {
			if (SSL_CTX_load_verify_locations(ctx, file, adr) != 1) {
				Logger::Print(RError, "SSL_CTX_load_verify_locations failed");
				exit(EXIT_FAILURE);
			}
		}
	}
};

int WebScraper::run(ParsedLink link, const size_t tries, const size_t redirect_limit) {
	int return_code;
	uint redirect_counter = 0;

	for (uint i = 0; i <= tries;) {
		//clear response
		response.clear();
		//run web scrapper
		bool secured = link.protocol == "https://";
		return_code = _run(link, secured);
		//exit from loop when done
		if (return_code == Error::OK)break;
		//check for connection error when secured and try again
		if (return_code == Error::CONNECTION_ERROR && secured) {
			link.protocol = "http://";
			Logger::Print(Runtime, "Failed to connect using SSL, trying HTTP");
			continue;
		}
		//check for critical errors
		if (return_code > Error::CRITICAL_ERROR) {
			Logger::Print(RError, "Establishing connection to '%s' failed", link.getLink().c_str());
			break;
		}
		//check for redirect
		if (return_code == Error::REDIRECT) {
			//check for redirect limit
			if (redirect_counter++ >= redirect_limit) {
				Logger::Print(RError, "Redirect limit reached for: '%s'", string{ link }.c_str());
				return_code = Error::REDIRECT_LIMIT;
				break;
			}
			string location = response.getHeader("Location");
			link = ParsedLink{ location };
			if (link.host.empty()) {
				Logger::Print(RError, "Failed to parse redirect location");
				return Error::RESPONSE_ERROR;
			}
			Logger::Print(Runtime, "Redirecting to '%s' [%d]", location.c_str(), redirect_counter);
			i = 0;
			continue;
		}
		//failed to retrieve data
		i++;
		if (i <= tries) {
			Logger::Print(Runtime, "Failed to retrieve data [%d], retrying ...", i);
		}
	}
	return return_code;
}

void append2Request(string& request, string text) {
	Logger::Print(Runtime, "\t%s", text.c_str());
	request += text + "\r\n";
}

int WebScraper::_run(ParsedLink link, bool secured) {
	link.port = link.port == "" ? (secured ? ":443" : ":80") : link.port;

	Wrapper wrap;
	Logger::Print(Runtime, "Connecting to '%s'", link.getLink().c_str());
	int return_code = wrap.setup(secured, ctx, link.getLink());
	if (return_code != 0) {
		return return_code;
	}

	//send request
	Logger::Print(Runtime, "Connected sending GET request:");

	string request;
	append2Request(request, "GET " + link.path + link.query + " HTTP/1.1");
	append2Request(request, "Host: " + link.host);
	append2Request(request, "Accept: */*");
	append2Request(request, "\r\n");

	if (BIO_write(wrap.getBio(), request.c_str(), request.length()) < 0) {
		if (BIO_should_retry(wrap.getBio())) {
			Logger::Print(RError, "While sending request");
			return Error::SEND_RETRY_ERROR;
		}
		else {
			Logger::Print(RError, "Failed to send request");
			return Error::SEND_ERROR;
		}
	}

	Logger::Print(Runtime, "GET request sent, waiting for response");
	//receive response
	const uint max_len = 1024;
	int len = max_len;
	char buffer[max_len] = "";
	//getHeader data length from header when available
	bool headerLoaded = false;
	//receive data until time out or end of data
	while ((len = BIO_read(wrap.getBio(), buffer, 1024)) > 0) {
		response.append(buffer, len);
		if (!headerLoaded) {
			if (response.isHeaderValid()) {
				//Logger::Print(Debug, "Response:\n%s", response.getAll());
				int r = response.getCode();
				switch (r)
				{
				case 301:
				case 302:
				case 303:
				case 307:
				case 308:
					Logger::Print(Runtime, "Revived redirect response [%d]", r);
					return Error::REDIRECT;
					break;
				case 200:
					Logger::Print(Runtime, "Revived response [%d]", r);
					break;
				case -1:
					Logger::Print(RError, "Failed to parse response header");
					return Error::RESPONSE_ERROR;
				default:
					Logger::Print(RError, "Revived bad response [%d]", r);
					return Error::BAD_RESPONSE;
				}

				Logger::Print(Runtime, "Fetching rest of data");
				headerLoaded = true;
			}
		}

		if (response.isDataValid()) break;
	}

	//check received data
	if (len <= 0) {
		if (len == 0) {
			Logger::Print(RError, "Connection closed");
			return Error::CONNECTION_CLOSED;
		}
		else if (BIO_should_retry(wrap.getBio())) {
			Logger::Print(RError, "Something went wrong while receiving data");
			return Error::RECEIVE_RETRY_ERROR;
		}
		else {
			Logger::Print(RError, "Failed to receive data");
			return Error::RECEIVE_ERROR;
		}
	}

	Logger::Print(Runtime, "Data received");
	Logger::Print(Debug, "Header valid:      %s", Bool2String(response.isHeaderValid()));
	Logger::Print(Debug, "Loaded data length:%5lu", response.getLoadedDataLenght());

	return Error::OK;
}