#include "app.h"


#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#include <clog.h>
#include "rules.h"



#define ERR_MSG_MAX_LEN 64


static const char *const OPTSTRING = "b:c:h:nr:R:vw:W";

/**
 * The long options array.
 */
static const struct option LONGOPTS[] = {
	{"board-width",  required_argument, NULL, 'w'},
	{"board-height", required_argument, NULL, 'h'},
	{"border-size",  required_argument, NULL, 'b'},
	{"cell-size",    required_argument, NULL, 'c'},
	{"no-border",    no_argument,       NULL, 'n'},
	{"game-rules",   required_argument, NULL, 'R'},
	{"update-rate",  required_argument, NULL, 'r'},
	{"vsync",        no_argument      , NULL, 'v'},
	{"wrap",         no_argument      , NULL, 'W'},
	{"", 0, NULL, 0}
};


static bool rvalset(const char *a, const char **dst) {
	const char *r = getRuleFromName(a);
	if (r != NULL) {
		*dst = r;
		return true;
	} else {
		regex_t re;
		const char *fmt = "B[0-9]*/S[0-9]*";
		bool valid;
		int status = regcomp(&re, fmt, REG_NOSUB);
		if (status != 0) {
			char err[ERR_MSG_MAX_LEN];
			regerror(status, &re, err, ERR_MSG_MAX_LEN);
			fatal("Could not compile regular expression for format \"%s\": %s", fmt, err);
			return false;
		}
		valid = regexec(&re, a, 0, NULL, 0) == 0;
		regfree(&re);
		if (valid) {
			*dst = a;
		} else {
			error("Invalid rules: \"%s\"", a);
		}
		return valid;
	}
}

static void getval(char opt, const char *arg, unsigned int *dst) {
	unsigned int tmp;
	if (sscanf(arg, "%u", &tmp) != 1) {
		error("Option -%c needs an unsigned integer argument", opt);
	} else {
		*dst = tmp;
	}
}

int parseCommandLineArgs(int argc, char **argv, unsigned int *board_width,
                         unsigned int *board_height, bool *wrap,
                         const char **game_rules, unsigned int *cell_pixels,
                         unsigned int *border_width, unsigned int *update_rate,
                         bool *use_vsync) {
	int ch, idx, res = 0, i;
	bool opt_r_met = false,
	     opt_v_met = false,
	     opt_b_met = false,
	     opt_n_met = false;
	while ((ch = getopt_long(argc, argv, OPTSTRING, LONGOPTS, &idx)) != -1) {
		switch (ch) {
			case 'b':
				getval('b', optarg, border_width);
				opt_b_met = true;
				break;
			case 'c':
				getval('c', optarg, cell_pixels);
				break;
			case 'h':
				getval('h', optarg, board_height);
				break;
			case 'n':
				*border_width = 0;
				opt_n_met = true;
				break;
			case 'r':
				getval('r', optarg, update_rate);
				opt_r_met = true;
				break;
			case 'R':
				if (!rvalset(optarg, game_rules)) {
					return -1;
				}
				break;
			case 'v':
				*use_vsync = true;
				opt_v_met = true;
				break;
			case 'w':
				getval('w', optarg, board_width);
				break;
			case 'W':
				*wrap = true;
				break;
			default:
				break;
		}
		res++;
	}
	if (opt_v_met && opt_r_met) {
		error("You cannot provide an update rate and ask to follow vSync at"
		      " the same time!");
		return -3;
	}
	if (opt_b_met && opt_n_met) {
		error("You cannot provide a border width and ask for no border between"
		      " the cells at the same time!");
		return -4;
	}
	for (i = optind; i < argc; ++i) {
		res++;
		warning("Unrecognized non-option argument \"%s\"", argv[i]);
	}
	return res == argc - 1 ? 0 : -5;
}
