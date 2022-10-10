#pragma once

#include <string_view>
#include <libxml/parser.h>

using namespace std;

class ParsedXML {
private:
	xmlDoc* doc = NULL;
	xmlNode* root_element = NULL;
public:
	ParsedXML(const string_view& link);

	bool isValid() { return validate(); };
private:
	bool validate();
};