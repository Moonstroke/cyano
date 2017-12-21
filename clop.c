#include "clop.h"


#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "log.h"


static unsigned int *_w, *_h, *_c, *_r, *_b;
static bool *_v, *_W;
static const char **_R;

void setvars(unsigned int *const w,
             unsigned int *const h,
             unsigned int *const c,
             unsigned int *const r,
             unsigned int *const b,
             bool *const v,
             bool *const W,
             const char **const R) {
	_w = w;
	_h = h;
	_c = c;
	_r = r;
	_b = b;
	_v = v;
	_W = W;
	_R = R;
}

static bool validate_rules(const char *const r) {
	const unsigned int b_pos = strchr(r, 'B') - r,
	                   s_pos = strchr(r, 'S') - r;
	const char *const slash_p = strchr(r, '/');
	unsigned int slash_pos = slash_p != NULL ? slash_p - r: s_pos - 1,
	             i;
	bool valid;
	debug("b_pos = %u, s_pos = %u, slash_pos = %u", b_pos, s_pos, slash_pos);
	valid = (b_pos == 0) && (slash_pos == (s_pos - 1)) && (s_pos > b_pos);
	debug("valid = %d", valid);
	for(i = 1; i < slash_pos && valid; ++i) {
		valid = '/' < r[i] && r[i] < ':';
		debug("i = %u, r[i] = '%c', valid = %d", i, r[i], valid);
		i++;
	}
	for(i = s_pos + 1; r[i] && valid; ++i) {
		valid = '/' < r[i] && r[i] < ':';
		debug("i = %u, r[i] = '%c', valid = %d", i, r[i], valid);
		i++;
	}
	return valid;
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
			case 'R':
				if(!validate_rules(optarg)) {
					error("Invalid rules: \"%s\"", optarg);
					return false;
				} else
					*_R = optarg;
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
