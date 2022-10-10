#pragma once

#include <string>

enum PrintMode
{
	Debug,
	DError,
	
	Runtime,
	RError
};
	
class Logger {
public:
	
	static void Print(const PrintMode type, const char* fmt, ...);
	
private:
	static void PrintTime();
	static void PrintType(const PrintMode type);
};

inline const char* Bool2String(bool b) {
	return b ? "true" : "false";
}

/// <summary>
/// trims white-spaces
/// </summary>
inline std::string trim(const std::string& str)
{
	const char* whitespace = " \t\n\r";
	size_t first = str.find_first_not_of(whitespace);
	if (first == std::string::npos) first = 0;
	size_t last = str.find_last_not_of(whitespace);
	return str.substr(first, (last - first + 1));
}