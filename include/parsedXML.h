#pragma once

#include <string_view>
#include <libxml/parser.h>

using namespace std;

class ParsedXML {
private:
	xmlDoc* doc = NULL;
	xmlNode* root = NULL;

	bool valid;
public:
	ParsedXML(const string_view& xml);
	~ParsedXML();

	bool isValid() { return valid; };
};

/// <summary>
/// Just initializes xmlParser and deinitializes when out of scope
/// </summary>
class XMLParserKeeper {
public:
	XMLParserKeeper() {
		xmlInitParser();
	}
	~XMLParserKeeper(){
		xmlCleanupParser();
	}
};