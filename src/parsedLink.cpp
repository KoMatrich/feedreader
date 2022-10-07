#include "parsedLink.h"

#include <string>

ParsedLink::ParsedLink(const string_view& link) {
	if (link.empty()) {
		// valid = false;
		return;
	}

	size_t hostIndex, pathIndex, queryIndex, portIndex;
	size_t offset, len;

	//order	fiter match_index
	//	|---"://"---|
	//	1	   ^ hostIndex 
	hostIndex = link.find("://");
	if (hostIndex == string::npos) hostIndex = 0; else hostIndex += 3;
	
	//	|---"://"-"/"-"/"-"?"-":"---|
	//	2		   ^	   ^   ^ portIndex
	//	3		   |	   | query index
	//	4		   |path index
	portIndex =	link.find(':', hostIndex);
	if (portIndex == string::npos) portIndex = link.length();
	
	queryIndex = link.find_last_of("?");
	if (queryIndex == string::npos) queryIndex = portIndex;
	
	pathIndex =	link.find('/', hostIndex);
	if (pathIndex == string::npos) pathIndex = queryIndex;

	offset = 0;
	len = hostIndex;
	protocol = string{ link.data() + offset, len};

	offset = hostIndex;
	len = pathIndex - offset;
	if (offset != string::npos) host = string{link.data() + offset, len};

	offset = pathIndex;
	len = queryIndex - offset;
	if (offset != string::npos) path = string{ link.data() + offset, len };

	offset = queryIndex;
	len = portIndex - offset;
	if (offset != string::npos) query = string{ link.data() + offset, len };

	offset = portIndex;
	len = link.length() - offset;
	if (offset != string::npos) port = string{ link.data() + offset};
}

bool ParsedLink::validate() {
	if (this->host.empty())
		return false;
	return true;
}