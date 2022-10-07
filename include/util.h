#pragma once

#include <stdarg.h>
#include <ctime>
#include <varargs.h>

#define DEBUG 1

/// release version error
#define R_ERROR(fmt,args...)\
do{\
	time_t now = time(0);\
	tm *ltm = localtime(&now);\
	fprintf(stderr,"[%d:%d:%d][Error]:"#fmt"\n",ltm->tm_hour,ltm->tm_min,ltm->tm_sec, ##args);\
} while (0);

#define R_PRINT(fmt,...)\
do{\
    time_t now = time(0);\
    tm *ltm = localtime(&now);\
    fprintf(stderr,"[%d:%d:%d][Info]: "#fmt"\n",ltm->tm_hour,ltm->tm_min,ltm->tm_sec, #__VA_ARGS__);\
} while (0);


#if DEBUG
/// debug version error
#define D_ERROR(fmt,args...)\
do{\
	time_t now = time(0);\
	tm *ltm = localtime(&now);\
	fprintf(stderr,"{%d:%d:%d}{Error}:"#fmt"\n",ltm->tm_hour,ltm->tm_min,ltm->tm_sec, ##args);\
} while (0);

/// debug version info
#define D_PRINT(fmt,args...)\
do{\
	time_t now = time(0);\
	tm *ltm = localtime(&now);\
	fprintf(stderr,"{%d:%d:%d}{Debug}:"#fmt"\n",ltm->tm_hour,ltm->tm_min,ltm->tm_sec, ##args);\
} while (0);

#define B(boolean)\
	((boolean) ? " true" : "false")
#else
#define	D_ERROR(fmt,args...) do{} while (0);
#define	D_PRINT(fmt,...) do{} while (0);
#define B(boolean) do{} while(0);
#endif
