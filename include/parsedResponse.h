#pragma once

#include <string>

using namespace std;

class PasedResponse {
public:
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
		return rawStorage.c_str() + headerLenght();
	}
	/// <summary>
	/// Returns data from header
	/// </summary>
	const string get(const char* optionName);
	/// <summary>
	/// Returns the length of data loaded from the response
	/// </summary>
	size_t loadedDataLenght();
	/// <summary>
	/// Gets data length from header
	/// </summary>
	/// <returns>
	/// string::npos if header is not valid
	/// </returns>
	size_t headerLenght();
	void clear() {
		rawStorage.clear();
	}
private:
	bool validHeader = false;
	bool validData = false;
	string rawStorage{};
};