#include "parsedXMLFeed.h"

#include "util.h"
#include <cstring>

bool isSame(const xmlChar* a, const char* b) {
	return strcmp((const char*)a, b) == 0;
}

const string_view getContent(const xmlNode* parent)
{
	while (parent->children != nullptr) {
		parent = parent->children;
	}
	if (parent->content == nullptr) return string_view{ "" };
	return string_view{ (const char*) parent->content };
}

ParsedXMLFeed::ParsedXMLFeed(const string_view& xml)
{
	//https://gnome.pages.gitlab.gnome.org/libxml2/devhelp/libxml2-parser.html#xmlParserOption
	uint options = 0;
	options |= XML_PARSE_NOBLANKS;
#ifndef DEBUG
	options |= XML_PARSE_NOERROR;
	options |= XML_PARSE_NOWARNING;
#endif // DEBUG

	//parse XML
	if (!(doc = xmlReadMemory(xml.data(), xml.length(), "XML", nullptr, options)))
	{
		Logger::Print(RError, "Failed to parse XML");
		valid = false;
		return;
	}

	//getHeader root element
	if (!(root = xmlDocGetRootElement(doc)))
	{
		Logger::Print(RError, "Failed to get root element");
		valid = false;
		return;
	}

	if (isSame(root->name, "rss")) {
		type = RSS;
	}
	else if (isSame(root->name, "atom")) {
		type = ATOM;
	}
	else {
		valid = false;
		return;
	}

	valid = true;
}

void ParsedXMLFeed::printAsFeed()
{
	for (auto ch = root->children; ch != nullptr; ch = ch->next) {
		if (!isSame(ch->name, "channel"))continue;
		for (auto i = ch->children; i != nullptr; i = i->next) {
			if (isSame(i->name, "title")) {
				printf("*** %s ***\n", getContent(i).data()); continue;
			}
			if (isSame(i->name, "item")) {
				Feed f;
				for (auto s = i->children; s != nullptr; s = s->next) {
					if (isSame(s->name, "title")) {
						f.addTitle(getContent(s)); continue;
					}
					if (isSame(s->name, "author")) {
						f.addAuthor(getContent(s)); continue;
					}
					if (isSame(s->name, "link")) {
						f.addLink(getContent(s)); continue;
					}
					if (isSame(s->name, "pubDate")) {
						f.addPubDate(getContent(s)); continue;
					}
				}
				f.print(true, true, true);
			}
		}
	}
}

ParsedXMLFeed::~ParsedXMLFeed()
{
	xmlFreeDoc(doc);
}