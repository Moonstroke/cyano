#include "app.h"


#include <getopt.h> /* for struct option, getopt_long, optarg, optind */
#include <stdio.h> /* for fprintf, stderr, sscanf, fputs */

#include "rules.h"



static const char *const OPTSTRING = "b:c:h:nr:R:vw:Wf:";

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
	{"file",         required_argument, NULL, 'f'},
	{"", 0, NULL, 0}
};


static int rvalset(const char *arg, const char **dst) {
	const char *rule = getRuleFromName(arg);
	if (rule != NULL) {
		*dst = rule;
		return 0;
	} else {
		if (arg[0] != 'B') {
			goto err;
		}
		size_t i = 1;
		/* Check that all chars until the slash are digits in ascending order */
		for (; '0' <= arg[i] && arg[i] <= '9'; ++i) {
			if (arg[i] >= arg[i + 1] && arg[i + 1] != '/' && arg[i + 1] != 'S') {
				goto err;
			}
		}
		if (arg[i] == '/') {
			++i;
		}
		if (arg[i++] != 'S') {
			goto err;
		}
		for (; '0' <= arg[i] && arg[i] <= '9'; ++i) {
			if(arg[i + 1] && arg[i] >= arg[i + 1]) {
				goto err;
			}
		}
		return 0;
err:
		fprintf(stderr, "Error: invalid rules: \"%s\"\n", arg);
		return -1;
	}
}

static int getval(char opt, const char *arg, unsigned int *dst) {
	unsigned int tmp;
	if (sscanf(arg, "%u", &tmp) != 1) {
		fprintf(stderr, "Error: option -%c needs an unsigned integer argument\n", opt);
		return -1;
	}
	*dst = tmp;
	return 0;
}

int parseCommandLineArgs(int argc, char **argv, unsigned int *board_width,
                         unsigned int *board_height, bool *wrap,
                         const char **game_rules, unsigned int *cell_pixels,
                         unsigned int *border_width, unsigned int *update_rate,
                         bool *use_vsync, const char **file) {
	int ch, idx, res = 0, i;
	bool opt_r_met = false,
	     opt_v_met = false,
	     opt_b_met = false,
	     opt_n_met = false;
	while ((ch = getopt_long(argc, argv, OPTSTRING, LONGOPTS, &idx)) != -1) {
		switch (ch) {
			case 'b':
				if (getval('b', optarg, border_width) < 0) {
					return -1;
				}
				opt_b_met = true;
				break;
			case 'c':
				if (getval('c', optarg, cell_pixels) < 0) {
					return -2;
				}
				break;
			case 'h':
				if (getval('h', optarg, board_height) < 0) {
					return -3;
				}
				break;
			case 'n':
				*border_width = 0;
				opt_n_met = true;
				break;
			case 'r':
				if (getval('r', optarg, update_rate) < 0) {
					return -4;
				}
				opt_r_met = true;
				break;
			case 'R':
				if (rvalset(optarg, game_rules) < 0) {
					return -5;
				}
				break;
			case 'v':
				*use_vsync = true;
				opt_v_met = true;
				break;
			case 'w':
				if (getval('w', optarg, board_width) < 0) {
					return -6;
				}
				break;
			case 'W':
				*wrap = true;
				break;
			case 'f':
				*file = optarg;
				break;
			default:
				break;
		}
		res++;
	}
	if (opt_v_met && opt_r_met) {
		fputs("Error: options --update-rate and --vsync are incompatible\n", stderr);
		return -7;
	}
	if (opt_b_met && opt_n_met) {
		fputs("Error: options --border-width and --no-border are incompatible\n", stderr);
		return -8;
	}
	for (i = optind; i < argc; ++i) {
		res++;
		fprintf(stderr, "Warning: skipping unrecognized non-option argument \"%s\"\n",
		        argv[i]);
	}
	return res == argc - 1 ? 0 : -9;
}
