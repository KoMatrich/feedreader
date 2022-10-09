#pragma once

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

