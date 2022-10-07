#include "pasedResponse.h"

#include <cstring>
#include <util.h>

void PasedResponse::append(const char* buffer,const size_t buffer_len)
{
	for (size_t i = 0; i < buffer_len; i++) {
		if(buffer[i] == '\0')break;
		rawStorage += buffer[i];
	}
}

bool PasedResponse::isHeaderValid()
{
	size_t endOfHeader = headerLenght();
	return endOfHeader == string::npos ? false : true;
}

bool PasedResponse::isDataValid()
{
	size_t endOfHeader = headerLenght();
	if (endOfHeader == string::npos) return false;
	size_t dataLenght = headerDataLenght();
	if (dataLenght == string::npos) return false;
	return rawStorage.length() - endOfHeader == dataLenght;
}

uint PasedResponse::getCode() {
	const char* prefix = " ";
	size_t codeIndex = rawStorage.find(prefix) + strlen(prefix);
	if (codeIndex == string::npos) -1;
	size_t codeEndIndex = rawStorage.find(" ", codeIndex);
	string codeS = rawStorage.substr(codeIndex, codeEndIndex - codeIndex);
	try
	{
		return stoi(codeS);
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

size_t PasedResponse::loadedDataLenght(){
	size_t endOfHeader = headerLenght();
	if (endOfHeader == string::npos) return 0;
	return rawStorage.length() - endOfHeader;
}

size_t PasedResponse::headerDataLenght()
{
	const char* prefix = "Content-Length: ";
	size_t contentLenghtIndex = rawStorage.find(prefix)+strlen(prefix);
	if (contentLenghtIndex == string::npos) return string::npos;
	size_t contentLenghtEndIndex = rawStorage.find("\r\n", contentLenghtIndex);
	string contentS = rawStorage.substr(contentLenghtIndex, contentLenghtEndIndex-contentLenghtIndex);
	try
	{
		return stoi(contentS);
	}
	catch (const std::exception&)
	{
		return string::npos;
	}
}

size_t PasedResponse::headerLenght()
{
	const char* headerSeparator = "\r\n\r\n";
	size_t endOfHeader = rawStorage.find(headerSeparator);
	if (endOfHeader == string::npos) return string::npos;
	return endOfHeader + strlen(headerSeparator);
}

