#include "clop.h"


#include <unistd.h>
#include <stdbool.h>

#include "log.h"



static unsigned int *_w, *_h, *_c, *_r, *_b;
static bool *_v, *_W;

void setvars(unsigned int *const w,
             unsigned int *const h,
             unsigned int *const c,
             unsigned int *const r,
             unsigned int *const b,
             bool *const v,
             bool *const W) {
	_w = w;
	_h = h;
	_c = c;
	_r = r;
	_b = b;
	_v = v;
	_W = W;
}


static void getval(const char opt, const char *const arg, unsigned int *const dst) {
	unsigned int tmp;
	if(sscanf(arg, "%u", &tmp) != 1)
		error("Option -%c needs an unsigned integer argument", opt);
	else
		*dst = tmp;
}
 
bool getvals(const int argc, const char *const argv[], const char *so, const struct option lo[]) {
	int ch, idx, res = 0, i;
	bool r_met = false, v_met = false, b_met = false, n_met = false;
	while((ch = getopt_long(argc, (char *const*)argv, so, lo, &idx)) != -1) {
		switch(ch) {
			case 'b':
				getval('b', optarg, _b);
				b_met = true;
				break;
			case 'c':
				getval('c', optarg, _c);
				break;
			case 'h':
				getval('h', optarg, _h);
				break;
			case 'n':
				*_b = 0;
				n_met = true;
				break;
			case 'r':
				getval('r', optarg, _r);
				r_met = true;
				break;
			case 'v':
				*_v = true;
				v_met = true;
				break;
			case 'w':
				getval('w', optarg, _w);
				break;
			case 'W':
				*_W = true;
				break;
			default:
				return false;
		}
		res++;
	}
	if(v_met && r_met) {
		error("You cannot provide an update rate and ask to follow vSync at the same time!");
		return false;
	}
	if(b_met && n_met) {
		error("You cannot provide a border width and ask for no border between the cells at the same time!");
		return false;
	}
	for(i = optind; i < argc; ++i) {
		res++;
		warning("Unrecognized non-option argument \"%s\"", argv[i]);
	}
	return res == argc - 1;
}
