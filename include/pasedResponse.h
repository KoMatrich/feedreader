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
	/// Checks if header is valid (complete). Doesn't check if all data has been recived.
	/// </summary>
	bool isHeaderValid();
	/// <summary>
	/// Checks if all data has been recived.
	/// </summary>
	bool isDataValid();
	/// <summary>
	/// Returns response code
	/// </summary>
	uint getCode();
	/// <summary>
	/// Returns pointer to whole request
	/// </summary>
	const char* raw() {
		return rawStorage.data();
	}
	/// <summary>
	/// Returns pointer to data of request
	/// </summary>
	const char* dat() {
		return rawStorage.data() + headerLenght();
	}
	/// <summary>
	/// Returns the lenght of data loaded from the response
	/// </summary>
	size_t loadedDataLenght();
	/// <summary>
	/// Gets data lenght from header
	/// </summary>
	/// <returns>
	/// string::npos if header is not valid
	/// </returns>
	size_t headerDataLenght();
	/// <summary>
	///	Length of header
	/// </summary>
	/// <returns>
	/// string::npos if header is not valid
	/// </returns>
	size_t headerLenght();
private:
	string rawStorage{};
};