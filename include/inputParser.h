#pragma once
#include "plagiat/getopt_b.h"
#include <string_view>

class optionParser {
public:
    std::string_view feedPath;  /// path to feed (web/file)
    bool WFlag; ///Flag to indicate feed type (true => web)

    std::string_view certFile;  /// path to certification file
    std::string_view certAddr;  /// path to dir of certification files

    bool TFlag; ///Flag to show time
    bool aFlag; ///Flag to show author
    bool uFlag; ///Flag to show URL
public:
	void run(int argc, char* argv[]);
};