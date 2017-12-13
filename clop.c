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


static void handleerr(const char optopt, const char *const valid_chars) {
	if(isprint(optopt)) {
		const char *c = valid_chars;
		bool recognized = false;
		while(*c && !recognized)
			recognized = *c++ == optopt;
		if(recognized)
			error("Option -%c requires an argument", optopt);
		else
			error("Unknow option -%c", optopt);
	}
	else
		error("Unknown option character 0x%x", optopt);
}

static void getval(const char opt, const char *const arg, unsigned int *const dst) {
	unsigned int tmp;
	if(sscanf(arg, "%u", &tmp) != 1)
		error("Option -%c needs an unsigned integer argument", opt);
	else
		*dst = tmp;
}
 
int getvals(const int argc, const char *const argv[], const char *s) {
	int c;
	while((c = getopt(argc, (char *const*)argv, s)) != -1) {
		switch(c) {
			case 'c':
				getval('c', optarg, _c);
				break;
			case 'w':
				getval('w', optarg, _w);
				break;
			case 'h':
				getval('h', optarg, _h);
				break;
			case '?':
				handleerr(optopt, s);
				return 1;
			default:
				return 2;
		}
	}
	int i;
	for(i = optind; i < argc; ++i)
		warning("Unrecognized non-option argument \"%s\"", argv[i]);

	return 0;
}
