#include <stdlib.h>
#include <stdio.h>
#include <openssl/ssl.h>

#include "inputParser.h"
#include "parsedLink.h"
#include "webScraper.h"
#include <iostream>
#include <util.h>

int main(int argc, char* argv[])
{
    optionParser oParser;
    oParser.run(argc, argv);

	//get certificates
	if(OPENSSL_init_ssl(OPENSSL_INIT_SSL_DEFAULT, nullptr)<0)
	{
		R_ERROR("Error: OPENSSL_init_ssl failed");
		exit(EXIT_FAILURE);
	}
	D_PRINT("%s", OpenSSL_version(OPENSSL_VERSION));
	
	//inicialize web scraper
	WebScraper WScraper(oParser.certAddr,oParser.certFile);
	
	//get links from file
	if (oParser.WFlag) {
	
	}
	
	//run web scraper
	do {
		string SLink;
		if (oParser.WFlag) {
			SLink = oParser.feedPath;
		} else {
			
		}
		
		ParsedLink link{ SLink };
		int return_code = WScraper.run(link, 3);
		if (return_code >= WebScraper::Error::CRITICAL_ERROR) {
			exit(EXIT_FAILURE);
		}
		else if (return_code == WebScraper::Error::OK) {
			std::cout << WScraper.response.raw();
		}
	} while (!oParser.WFlag);
	
    exit(EXIT_SUCCESS);
}