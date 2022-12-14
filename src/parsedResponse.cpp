#include "parsedResponse.h"

#include <cstring>
#include <fstream>

#include "util.h"

//end of line
const char* EOL = "\r\n";
//htpp response separator
const char* SEPARATOR = "\r\n\r\n";

PasedResponse::~PasedResponse()
{
#ifdef DEBUG
	ofstream f{ "ParsedResponseDebug.txt" };
	f.write(debug.c_str(), debug.length());
	f.close();
#endif // DEBUG
}

void PasedResponse::append(const char* buffer,const size_t buffer_len)
{
	for (size_t i = 0; i < buffer_len; i++) {
		if(buffer[i] == '\0')break;
		rawStorage += buffer[i];
#ifdef DEBUG
		debug += buffer[i];
#endif // DEBUG
	}
}

bool PasedResponse::isHeaderValid()
{
	if (validHeader) return true;
	
	size_t endOfHeader = getHeaderLenght();

	validHeader = endOfHeader == string::npos ? false : true;
	return validHeader;
}

size_t PasedResponse::predictDataLenght() {
	{
		static bool inicialized = false;
		
		if (!validHeader) return -1;
		if (!inicialized) {
			string contentLenght = getHeader("Content-Length");
			
			if (contentLenght.empty()) segmented = true;
			else {
				predictedDataLenght = std::stoi(contentLenght);
				inicialized = true;
				return predictedDataLenght;
			}
		}

		if (segmented) {
			if (getLoadedDataLenght() >= predictedDataLenght || !inicialized) {
				//find data length info in data and remove it
				size_t dataIndex = getHeaderLenght() + predictedDataLenght;
				size_t endOfLine = rawStorage.find(EOL, dataIndex);
				if (endOfLine == string::npos) return predictedDataLenght;

				string data = rawStorage.substr(dataIndex, endOfLine - dataIndex);
				if (data.empty()) return predictedDataLenght;
				
				predictedDataLenght += std::stoi(data, 0, 16);
				rawStorage.erase(dataIndex, endOfLine - dataIndex + strlen(EOL));
				predictedDataLenght += strlen(EOL);
			}
		}

		inicialized = true;
		return predictedDataLenght;
	}
}

void PasedResponse::RemoveEndCode() {
	size_t endSeparator = rawStorage.find(SEPARATOR,getHeaderLenght());
	if (endSeparator == string::npos) return;
	rawStorage.erase(endSeparator, rawStorage.length()-endSeparator);
}

bool PasedResponse::isDataValid()
{
	if (validData) return true;
	if (!validHeader) return false;
	
	size_t len = predictDataLenght();
	validData = getLoadedDataLenght() >= len;
	
	if(validData)
		RemoveEndCode();
	
	return validData;
}

uint PasedResponse::getCode() {
	const char* prefix = " ";
	size_t codeIndex = rawStorage.find(prefix) + strlen(prefix);
	if (codeIndex == string::npos) return -1;
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

const string PasedResponse::getHeader(const char* optionName)
{
	string prefix = EOL;
	prefix += optionName;
	prefix += ": ";
	
	//TODO limit search range to header only
	size_t redirectIndex = rawStorage.find(prefix);
	if (redirectIndex == string::npos) return "";
	redirectIndex += prefix.length();

	size_t redirectEndIndex = rawStorage.find(EOL, redirectIndex);
		
	string data = rawStorage.substr(redirectIndex, redirectEndIndex - redirectIndex);
	return data;
}

size_t PasedResponse::getLoadedDataLenght(){
	size_t endOfHeader = getHeaderLenght();
	if (endOfHeader == string::npos) return 0;
	return rawStorage.length() - endOfHeader;
}

size_t PasedResponse::getHeaderLenght()
{
	size_t endOfHeader = rawStorage.find(SEPARATOR);
	if (endOfHeader == string::npos) return string::npos;
	return endOfHeader + strlen(SEPARATOR);
}