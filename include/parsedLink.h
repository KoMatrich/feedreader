#pragma once

#include <string_view>
#include <string>

using namespace std;

class ParsedLink {
public:	
	string protocol;
	string host;
	string path;
	string query;
	string port;

	ParsedLink(const string_view& link);	

	bool isValid() {return validate(); };
private:
	bool validate();
};