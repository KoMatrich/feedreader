#include <stdlib.h>
#include <stdio.h>
#include <openssl/ssl.h>

#include "inputParser.h"
#include "parsedLink.h"
#include "webScraper.h"
#include <iostream>
#include <util.h>
#include <vector>

int main(int argc, char* argv[])
{
    optionParser oParser;
    oParser.run(argc, argv);

	//get certificates
	if(OPENSSL_init_ssl(OPENSSL_INIT_SSL_DEFAULT, nullptr)<0)
	{
		Logger::Print(RError, "OPENSSL_init_ssl failed");
		exit(EXIT_FAILURE);
	}
	
	Logger::Print(Debug, "%s", OpenSSL_version(OPENSSL_VERSION));
	
	//inicialize web scraper
	WebScraper WScraper(oParser.certAddr,oParser.certFile);
	
	vector<string> links;
	
	//get links from file
	if (oParser.WFlag) {
		links.push_back(oParser.feedPath.data());
	}else {
		
	}
	
	//run web scraper
	do {
		string SLink = links.back();
		links.pop_back();
		
		ParsedLink link{ SLink };
		int return_code = WScraper.run(link,3,3);
		std::cout << WScraper.response.raw();
		if (return_code != WebScraper::Error::OK) continue;
	} while (!links.empty());
	
    exit(EXIT_SUCCESS);
}