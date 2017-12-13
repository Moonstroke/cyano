#include "clop.h"


#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

#include "log.h"



static unsigned int *_w, *_h, *_c;


void setvars(unsigned int *const w, unsigned int *const h, unsigned int *const c) {
	_w = w;
	_h = h;
	_c = c;
}


static void getval(const char opt, const char *const arg, unsigned int *const dst) {
	unsigned int tmp;
	if(sscanf(arg, "%u", &tmp) != 1)
		error("Option -%c needs an unsigned integer argument", opt);
	else
		*dst = tmp;
}
 
bool getvals(const int argc, const char *const argv[], const char *so, const struct option lo[]) {
	int ch, idx, res = 0;
	while((ch = getopt_long(argc, (char *const*)argv, so, lo, &idx)) != -1) {
		switch(ch) {
			case 'c':
				getval('c', optarg, _c);
				break;
			case 'w':
				getval('w', optarg, _w);
				break;
			case 'h':
				getval('h', optarg, _h);
				break;
			case '\0':
				// found flag option
				break;
			case '?':
				return 1;
			default:
				return 2;
		}
		res++;
	}
	int i;
	for(i = optind; i < argc; ++i) {
		res++;
		warning("Unrecognized non-option argument \"%s\"", argv[i]);
	}
	return res == argc - 1;
}
