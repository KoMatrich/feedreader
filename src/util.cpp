#include <stdarg.h>
#include <ctime>
#include <stdio.h>

#include "util.h"

#define DEBUG 1

void Logger::Print(const PrintMode type, const char* fmt, ...)
{
#ifndef DEBUG
	if (type < Runtime)return;
#endif // !DEBUG

	va_list valist;
	va_start(valist, fmt);

	PrintTime();
	PrintType(type);

	vfprintf(stderr, fmt, valist);
	fprintf(stderr, "\n");
}

void Logger::PrintTime()
{
	time_t now = time(0);
	tm* ltm = localtime(&now);

	fprintf(stderr, "[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

void Logger::PrintType(const PrintMode type) {
	switch (type)
	{
	case Debug:
		fprintf(stderr, "{DEBUG}");
		break;
	case DError:
		fprintf(stderr, "{ERROR}");
		break;
	case Runtime:
		fprintf(stderr, "[INFO] ");
		break;
	case RError:
		fprintf(stderr, "[ERROR]");
		break;
	}
}
