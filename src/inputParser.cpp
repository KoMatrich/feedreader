#include "inputParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>

void optionParser::run(int argc, char* argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Missing options. Use \"-h\" for help.");
        exit(EXIT_FAILURE);
    }

    //skip script name
	//use next arg as default
    WFlag = true;
    feedPath = argv[1];
	
    int opt;
    //https://www.man7.org/linux/man-pages/man3/getopt.3.html
    while ((opt = getopt(argc, argv, ":f:c:CTauh")) != -1) {
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
            fprintf(stderr, "       folder where to seach for certfiles\n");
            fprintf(stderr, "   if -c or -C option is used:\n");
            fprintf(stderr, "       then certificates from default storage will be used\n");
            fprintf(stderr, "\n");
            fprintf(stderr, "   -T\n");
            fprintf(stderr, "       shows time informations for each records (if aviable)\n");
            fprintf(stderr, "   -a\n");
            fprintf(stderr, "       shows authors name or email for each record (if aviable)\n");
            fprintf(stderr, "   -u\n");
            fprintf(stderr, "       shows asociated URL for each record (if aviable)\n");
            exit(EXIT_SUCCESS);
        case ':':
            fprintf(stderr, "option needs a value");
            exit(EXIT_FAILURE);
        case '?':
            fprintf(stderr, "unknown option: %c", optopt);
            exit(EXIT_FAILURE);
        };
    }
}