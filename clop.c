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

static int setvals(const char *ws, const char *hs, const char *cs) {
	return sscanf(ws, "%u", _w) == 1
	    && sscanf(hs, "%u", _h) == 1
	    && sscanf(cs, "%u", _c) == 1;
}

int getvals(const int argc, const char *const argv[]) {
	const char *cs, *ws, *hs;
	int c;
	while((c = getopt(argc, (char *const*)argv, OPTSTRING)) != -1) {
		switch(c) {
			case 'c':
				cs = optarg;
				break;
			case 'w':
				ws = optarg;
				break;
			case 'h':
				hs = optarg;
				break;
			case '?':
				handleerr(optopt, OPTSTRING);
				return 1;
			default:
				return 2;
		}
	}
	if(setvals(ws, hs, cs) != 0)
		return 3;
	int i;
	for(i = optind; i < argc; ++i)
		warning("Unrecognized non-option argument \"%s\"", argv[i]);

	return 0;
}
