#include "inputParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include "util.h"

void optionParser::run(int argc, char* argv[]) {
    if (argc <= 1) {
        Logger::Print(RError,"Missing options. Use \"-h\" for help.");
        exit(EXIT_FAILURE);
    }

    //skip script name
	//use next arg as default
    WFlag = true;
    feedPath = argv[1];
	
    int opt;
    //https://www.man7.org/linux/man-pages/man3/getopt.3.html
    while ((opt = getopt(argc, argv, "f:c:C:Tauh")) != -1) {
        switch (opt) {
        case 'f':
            feedPath = optarg;
            WFlag = false;
            break;
        case 'c':
            certFile = optarg;
            break;
        case 'C':
            certAddr = optarg;
            break;
        case 'T':
            TFlag = true;
            break;
        case 'a':
            aFlag = true;
            break;
        case 'u':
            uFlag = true;
            break;
        case 'h':
            fprintf(stderr, "Usage:\n");
            fprintf(stderr, "   feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]\n");
            fprintf(stderr, "\n");
            fprintf(stderr, "   URL can be http or https\n");
            fprintf(stderr, "\n");
            fprintf(stderr, "   -f <feedfile>\n");
            fprintf(stderr, "       specifies location of input file\n");
            fprintf(stderr, "       feedfile is formated as Atom or RSS\n");
            fprintf(stderr, "\n");
            fprintf(stderr, "   -c <certfile>\n");
            fprintf(stderr, "       certfile is used for SSL/TLS\n");
            fprintf(stderr, "   -C <certaddr>\n");
            fprintf(stderr, "       folder where to search for certfiles\n");
            fprintf(stderr, "   if -c or -C option is used:\n");
            fprintf(stderr, "       then certificates from default storage will be used\n");
            fprintf(stderr, "\n");
            fprintf(stderr, "   -T\n");
            fprintf(stderr, "       shows time informations for each records (if available)\n");
            fprintf(stderr, "   -a\n");
            fprintf(stderr, "       shows authors name or email for each record (if available)\n");
            fprintf(stderr, "   -u\n");
            fprintf(stderr, "       shows associated URL for each record (if available)\n");
            exit(EXIT_SUCCESS);
        case ':':
            Logger::Print(RError, "option needs a value");
            exit(EXIT_FAILURE);
        case '?':
            Logger::Print(RError, "unknown option: %c", optopt);
            exit(EXIT_FAILURE);
        };
    }
    if (feedPath.empty()) {
        Logger::Print(RError, "missing feed URL");
        exit(EXIT_FAILURE);
    }
}