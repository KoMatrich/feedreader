#include <stdlib.h>
#include <stdio.h>
#include <openssl/ssl.h>
#include <iostream>
#include <util.h>
#include <vector>
#include <fstream>

#include "inputParser.h"
#include "parsedLink.h"
#include "webScraper.h"
#include <parsedXML.h>

int main(int argc, char* argv[])
{
	optionParser oParser;
	oParser.run(argc, argv);

	//get certificates
	if (OPENSSL_init_ssl(OPENSSL_INIT_SSL_DEFAULT, nullptr) < 0)
	{
		Logger::Print(RError, "OPENSSL_init_ssl failed");
		return(EXIT_FAILURE);
	}

	Logger::Print(Debug, "%s", OpenSSL_version(OPENSSL_VERSION));

	LIBXML_TEST_VERSION	//check libxml version

	//inicialize web scraper
	WebScraper WScraper(oParser.certAddr, oParser.certFile);

	vector<string> links;

	//get links from file
	if (oParser.WFlag) {
		Logger::Print(Runtime, "Using entered link to get feed");
		links.push_back(oParser.feedPath.data());
	}
	else {
		Logger::Print(Runtime, "Using file containg links to get feed");
		const char* fileName = oParser.feedPath.data();
		std::ifstream file{ fileName };
		if (!file.is_open()) {
			Logger::Print(RError, "Error opening file '%s'", fileName);
			return(EXIT_FAILURE);
		}
		
		Logger::Print(Runtime, "Loading links from file");
		string line;
		while (std::getline(file, line)) {
			line = trim(line);
			if (line.empty()) continue;
			if (line.at(0) == '#') continue;
			links.push_back(line);
		}
		file.close();
		Logger::Print(Runtime, "All links loaded");
	}

	//run web scraper
	Logger::Print(Runtime, "Scraping for feed/s");
	do {
		string SLink = links.back();
		links.pop_back();

		ParsedLink link{ SLink };
		int return_code = WScraper.run(link, 3, 3);
		
		if (return_code != WebScraper::Error::OK) continue;
		ParsedXML xml{WScraper.response.dat()};
	} while (!links.empty());

	return(EXIT_SUCCESS);
}