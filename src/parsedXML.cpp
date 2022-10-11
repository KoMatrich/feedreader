#include "parsedXML.h"

#include "util.h"

ParsedXML::ParsedXML(const string_view& xml)
{	
	//https://gnome.pages.gitlab.gnome.org/libxml2/devhelp/libxml2-parser.html#xmlParserOption
	uint options = 0;
	options |= XML_PARSE_NOBLANKS;
	//options |= XML_PARSE_NOERROR;
	//options |= XML_PARSE_NOWARNING;
	
	//parse XML
	if (!(doc = xmlReadFile(xml.data(), NULL, options)))
	{
		Logger::Print(RError, "Failed to parse XML");
		valid = false;
		return;
	}
	
	//get root element
	if (!(root = xmlDocGetRootElement(doc)))
	{
		Logger::Print(RError, "Failed to get root element");
		valid = false;
		return;
	}

	valid = true;
}

ParsedXML::~ParsedXML()
{
	xmlFreeDoc(doc);
}
