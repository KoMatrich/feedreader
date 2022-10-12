#pragma once

#include <string_view>
#include <libxml/parser.h>

using namespace std;

enum xmlType {
	None,
	RSS,
	ATOM,
};

class ParsedXMLFeed {
private:
	xmlDoc* doc = NULL;
	xmlNode* root = NULL;

	bool valid;
	xmlType type;
public:
	ParsedXMLFeed(const string_view& xml);
	~ParsedXMLFeed();

	bool isValid() { return valid; };

	void printAsFeed();
};

/// <summary>
/// Just initializes xmlParser and deinitializes when out of scope
/// </summary>
class XMLParserKeeper {
public:
	XMLParserKeeper() {
		xmlInitParser();
	}
	~XMLParserKeeper() {
		xmlCleanupParser();
	}
};

class Feed {
private:
	string_view title = "";
	string_view author = "";
	string_view link = "";
	string_view pubDate = "";
	
public:
	void addTitle(string_view input) {
		if (input.empty()) return;
		title = input;
	}
	
	void addAuthor(string_view input) {
		if (input.empty()) return;
		author = input;
	}
	
	void addLink(string_view input) {
		if (input.empty()) return;
		link = input;
	}
	
	void addPubDate(string_view input) {
		if (input.empty()) return;
		pubDate = input;
	}

	void print(bool l,bool a, bool t) {
		printf("%s\n", title.data());
		if (!link.empty()&&l)		printf("URL: %s\n", link.data());
		if (!author.empty()&&a)		printf("Autor: %s\n", author.data());
		if (!pubDate.empty()&&t)	printf("Aktualizace: %s\n", pubDate.data());
		if (l || a || t)			printf("\n");
	}
};
