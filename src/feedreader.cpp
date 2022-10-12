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
#include <parsedXMLFeed.h>

int main(int argc, char* argv[])
{
	vector<string> links;
	optionParser oParser;
	oParser.run(argc, argv);
	
	//input processing part and initialization
	{
		//getHeader certificates
		if (OPENSSL_init_ssl(OPENSSL_INIT_SSL_DEFAULT, nullptr) < 0)
		{
			Logger::Print(RError, "OPENSSL_init_ssl failed");
			return(EXIT_FAILURE);
		}

		Logger::Print(Debug, "%s", OpenSSL_version(OPENSSL_VERSION));
		LIBXML_TEST_VERSION;//check libxml version

		//getHeader links from file
		if (oParser.WFlag) {
			Logger::Print(Runtime, "Using entered link to get feed");
			links.push_back(oParser.feedPath.data());
		}
		else {
			Logger::Print(Runtime, "Using file that contains links to get feed");
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
	}	
	
	//processing link/s
	{
		//initialize parsedXML
		XMLParserKeeper keep;

		//initialize web scraper
		WebScraper WScraper(oParser.certAddr, oParser.certFile);

		//run web scraper
		Logger::Print(Runtime, "Scraping for feed/s");
		do {
			string SLink = links.back();
			links.pop_back();

			ParsedLink link{ SLink };
			int return_code = WScraper.run(link, 3, 3);
			if (return_code != WebScraper::Error::OK) continue;
			ParsedXMLFeed xml{ WScraper.response.getData() };
			if (!xml.isValid()) {
				Logger::Print(RError, "Failed to parse XML");
				continue;
			}
			xml.printAsFeed(true,true,true);
			//xml.printAsFeed(oParser.uFlag, oParser.aFlag, oParser.TFlag);
			
		} while (!links.empty());
	}
	return(EXIT_SUCCESS);
}