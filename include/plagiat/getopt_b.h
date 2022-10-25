// fix potential missing header file (WSL problem)
// from xkrizv03@vutbr.cz

#if __has_include(<getopt.h>)
#include <getopt.h>
#else
#include <res/getopt_w.h>
#endif