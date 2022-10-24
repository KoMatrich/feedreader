#pragma once

#include <string>

using namespace std;

class PasedResponse {
public:
	~PasedResponse();
	/// <summary>
	/// Adds data to the response
	/// </summary>
	void append(const char* buffer,const size_t buffer_len);
	/// <summary>
	/// Checks if header is valid (complete). Doesn't check if all data has been received.
	/// </summary>
	bool isHeaderValid();
	/// <summary>
	/// Checks if all data has been received.
	/// </summary>
	bool isDataValid();
	/// <summary>
	/// Returns response code
	/// </summary>
	uint getCode();
	/// <summary>
	/// Returns pointer to whole request
	/// </summary>
	const char* getAll() {
		return rawStorage.c_str();
	}
	/// <summary>
	/// Returns pointer to data of request
	/// </summary>
	const char* getData() {
		return rawStorage.c_str() + getHeaderLenght();
	}
	/// <summary>
	/// Returns data from header
	/// </summary>
	const string getHeader(const char* optionName);
	/// <summary>
	/// Returns the length of data loaded from the response
	/// </summary>
	size_t getLoadedDataLenght();
	/// <summary>
	/// Gets data length from header
	/// </summary>
	/// <returns>
	/// string::npos if header is not valid
	/// </returns>
	size_t getHeaderLenght();
	/// <summary>
	/// Resets the response
	/// </summary>
	void clear() {
		rawStorage.clear();
		validHeader = false;
		validData = false;
		segmented = false;
	}
private:
	/// <summary>
	/// Removes return code if available at end of response
	/// </summary>
	void RemoveEndCode();
	size_t predictDataLenght();
	bool validHeader, validData, segmented;
	size_t predictedDataLenght = 0;
	string rawStorage{};

#ifdef DEBUG
	string debug{};
#endif // DEBUG
};