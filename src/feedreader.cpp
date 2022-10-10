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

	//inicialize web scraper
	WebScraper WScraper(oParser.certAddr, oParser.certFile);

	vector<string> links;

	//get links from file
	if (oParser.WFlag) {
		Logger::Print(Runtime, "");
		links.push_back(oParser.feedPath.data());
	}
	else {
		const char* fileName = oParser.feedPath.data();
		std::ifstream file{ fileName };
		if (!file.is_open()) {
			Logger::Print(RError, "Error opening file '%s'", fileName);
			return(EXIT_FAILURE);
		}
		
		string line;
		while (std::getline(file, line)) {
			line = trim(line);
			if (line.empty()) continue;
			if (line.at(0) == '#') continue;
			links.push_back(line);
		}
		file.close();
	}

	//run web scraper
	do {
		string SLink = links.back();
		links.pop_back();

		ParsedLink link{ SLink };
		int return_code = WScraper.run(link, 3, 3);
		std::ofstream outfile{ "outfile.txt" };
		outfile << WScraper.response.raw();
		if (return_code != WebScraper::Error::OK) continue;
	} while (!links.empty());

	return(EXIT_SUCCESS);
}