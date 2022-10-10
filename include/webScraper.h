#pragma once
#include <netinet/in.h>
#include <string_view>
#include <openssl/ssl.h>

#include "parsedLink.h"
#include "parsedResponse.h"

using namespace std;

class WebScraper {
private:
	SSL_CTX* ctx;

	int _run(ParsedLink link);
public:
	WebScraper(const string_view certAddr,const string_view certFile);
	~WebScraper()
	{
		SSL_CTX_free(ctx);
	}
	/// <summary>
	/// Runs scraper for given link
	/// </summary>
	int run(ParsedLink link, const size_t tries = 3, const size_t redirect_limit = 3);
	
	PasedResponse response;

	enum Error {
		OK,

		CONNECTION_CLOSED,
		
		SEND_RETRY_ERROR,
		RECEIVE_RETRY_ERROR,

		RESPONSE_ERROR,

		REDIRECT,
		
		CRITICAL_ERROR,/// used only to compare types
		
		BAD_RESPONSE,

		CONNECTION_ERROR,

		SSL_ERROR,

		SEND_ERROR,
		RECEIVE_ERROR,

		HEADER_ERROR,
		
		REDIRECT_LIMIT
	};
};