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
	
	operator std::string() const {
		return protocol + "://" + host + path + query + port;
	}

	const string getLink() const{
		return host + port;
	}

	bool isValid() {return validate(); };
private:
	bool validate();
};