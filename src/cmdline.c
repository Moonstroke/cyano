#include "app.h"


#ifdef _GNU_SOURCE
# include <getopt.h> /* for struct option, getopt_long, optarg, optind */
#else
enum arg_type {
	no_argument,
	required_argument
};

struct option {
	char *name;
	enum arg_type arg_type;
	void *flag;
	char val;
};

static char *optarg;
static int optopt;
static int opterr;
static int optind;

/* Forward declaration, definition at bottom of file */
int getopt_long(int argc, char *const argv[], const char *optstring,
                const struct option *longopts, int *longindex);
#endif
#include <stdio.h> /* for fprintf, stderr, sscanf, fputs */
#include <string.h> /* for strcasecmp / _stricmp */

#include "rules.h"



/* Split into two strings because only the first line needs formatting */
static const char USAGE_HEADER[] = "Usage: %s [OPTION]...\n";
static const char USAGE[] = "where OPTION is any of the following:\n"
	"\t-w WIDTH, --grid-width=WIDTH\n"
	"\t\tSpecify the width of the grid (integer argument, default 80)\n"
	"\t-h HEIGHT, --grid-height=HEIGHT\n"
	"\t\tSpecify the height of the grid (integer arg, default 60)\n"
	"\t-c CELL_SIZE, --cell-size=CELL_SIZE\n"
	"\t\tSpecify the size of each cell (integer arg, default 16)\n"
	"\t-b BORDER_SIZE, --border-size=BORDER_SIZE\n"
	"\t\tSpecify the width of the gap between the cells (integer arg, default "
	"1)\n"
	"\t-n, --no-border\n"
	"\t\tDisable the gap beetween the cells\n"
	"\t-R GAME_RULE, --game-rule=GAME_RULE\n"
	"\t\tSpecify the rulestring or name of the variant to run (string arg, "
	"default \"B3/S23\")\n"
	"\t-r UPDATE_RATE, --update-rate=UPDATE_RATE\n"
	"\t\tSpecify the number of generations to compute per second (integer arg, "
	"default 25)\n"
	"\t-v, --vsync\n"
	"\t\tSpecify to match the update rate with the monitor refresh rate\n"
	"\t-W, --wrap\n"
	"\t\tSpecify to make the grid wrap (opposite sides connect)\n"
	"\t-f FILE, --file=FILE\n"
	"\t\tSpecify the file for input and output (string arg, default none)\n"
	"\t-i INPUT_FILE, --input-file=INPUT_FILE\n"
	"\t\tSpecify the file for input (string argument, default none)\n"
	"\t-o OUTPUT_FILE, --output-file=OUTPUT_FILE\n"
	"\t\tSpecify the file for output (string argument, default none)\n"
	"\t-F FORMAT, --format=FORMAT\n"
	"\t\tSpecify the grid representation format in the input file: one of "
	"\"plain\", \"plaintext\" or \"RLE\" case not significant(string argument, "
	"default none)\n"
	"\t--help, --usage\n"
	"\t\tPrint this message and exit\n";


static const char *const OPTSTRING = ":b:c:h:nr:R:vw:Wf:i:o:F:";

/**
 * The long options array.
 */
static const struct option LONGOPTS[] = {
	{"grid-width",  required_argument, NULL, 'w'},
	{"grid-height", required_argument, NULL, 'h'},
	{"border-size", required_argument, NULL, 'b'},
	{"cell-size",   required_argument, NULL, 'c'},
	{"no-border",   no_argument,       NULL, 'n'},
	{"game-rule",   required_argument, NULL, 'R'},
	{"update-rate", required_argument, NULL, 'r'},
	{"vsync",       no_argument      , NULL, 'v'},
	{"wrap",        no_argument      , NULL, 'W'},
	{"file",        required_argument, NULL, 'f'},
	{"input-file",  required_argument, NULL, 'i'},
	{"output-file", required_argument, NULL, 'o'},
	{"usage",       no_argument      , NULL, 'u'},
	{"help",        no_argument      , NULL, 'u'},
	{"format",      required_argument, NULL, 'F'},
	{"", 0, NULL, 0}
};


static int _print_usage(const char *argv0) {
	fprintf(stderr, USAGE_HEADER, argv0);
	fwrite(USAGE, sizeof USAGE, 1, stderr);
	return -__LINE__;
}

static int _set_rule(const char *arg, const char **dst) {
	const char *rule = get_rule_from_name(arg);
	if (rule != NULL) {
		*dst = rule;
		return 0;
	} else {
		if (arg[0] != 'B') {
			goto err;
		}
		size_t i = 1;
		/* Check that all chars until the slash are digits in ascending order */
		for (; '0' <= arg[i] && arg[i] <= '8'; ++i) {
			if (arg[i] >= arg[i + 1] && arg[i + 1] != '/'
			                         && arg[i + 1] != 'S') {
				goto err;
			}
		}
		if (arg[i] == '/') {
			++i;
		}
		if (arg[i++] != 'S') {
			goto err;
		}
		for (; '0' <= arg[i] && arg[i] <= '8'; ++i) {
			if(arg[i + 1] && arg[i] >= arg[i + 1]) {
				goto err;
			}
		}
		*dst = arg;
		return 0;
err:
		fprintf(stderr, "Error: invalid rule: \"%s\"\n", arg);
		return -__LINE__;
	}
}

static int _get_uint_value(char opt, const char *arg, unsigned int *dst) {
	unsigned int tmp;
	if (sscanf(arg, "%u", &tmp) != 1) {
		fprintf(stderr,
		        "Error: option -%c needs an unsigned integer argument\n", opt);
		return -__LINE__;
	}
	*dst = tmp;
	return 0;
}

static void _parse_format(const char *arg, enum grid_format *format) {
	int (*cmp_func)(const char*, const char*);
#ifdef _MSC_VER
	cmp_func = _stricmp;
#else
	cmp_func = strcasecmp;
#endif
	if (cmp_func(arg, "RLE") == 0) {
		*format = GRID_FORMAT_RLE;
	} else if (cmp_func(arg, "plaintext") == 0 || cmp_func(arg, "plain") == 0) {
		*format = GRID_FORMAT_PLAIN;
	} else {
		fprintf(stderr, "Warning: unrecognized grid representationformat: "
		                "\"%s\"\n", optarg);
		*format = GRID_FORMAT_UNKNOWN;
	}
}

int parse_cmdline(int argc, char **argv, unsigned int *grid_width,
                  unsigned int *grid_height, bool *wrap, const char **game_rule,
                  unsigned int *cell_pixels, unsigned int *border_width,
                  unsigned int *update_rate, bool *use_vsync,
                  const char **in_file, const char **out_file,
                  enum grid_format *format) {
	bool opt_r_met = false;
	bool opt_v_met = false;
	bool opt_b_met = false;
	bool opt_n_met = false;
	bool opt_w_met = false;
	bool opt_h_met = false;
	bool opt_f_met = false;
	bool opt_i_met = false;
	bool opt_o_met = false;
	int ch;
	int idx;
	optind = 1;
	opterr = 0;
	while ((ch = getopt_long(argc, argv, OPTSTRING, LONGOPTS, &idx)) != -1) {
		switch (ch) {
			case 'u':
				_print_usage(argv[0]);
				return 1;
			case 'b':
				if (_get_uint_value('b', optarg, border_width) < 0) {
					return -__LINE__;
				}
				opt_b_met = true;
				break;
			case 'c':
				if (_get_uint_value('c', optarg, cell_pixels) < 0) {
					return -__LINE__;
				}
				break;
			case 'h':
				if (_get_uint_value('h', optarg, grid_height) < 0) {
					return -__LINE__;
				}
				opt_h_met = true;
				break;
			case 'n':
				*border_width = 0;
				opt_n_met = true;
				break;
			case 'r':
				if (_get_uint_value('r', optarg, update_rate) < 0) {
					return -__LINE__;
				}
				opt_r_met = true;
				break;
			case 'R':
				if (_set_rule(optarg, game_rule) < 0) {
					return -__LINE__;
				}
				break;
			case 'v':
				*use_vsync = true;
				opt_v_met = true;
				break;
			case 'w':
				if (_get_uint_value('w', optarg, grid_width) < 0) {
					return -__LINE__;
				}
				opt_w_met = true;
				break;
			case 'W':
				*wrap = true;
				break;
			case 'f':
				*in_file = *out_file = optarg;
				opt_f_met = true;
				break;
			case 'i':
				*in_file = optarg;
				opt_i_met = true;
				break;
			case 'o':
				*out_file = optarg;
				opt_o_met = true;
				break;
			case 'F':
				_parse_format(optarg, format);
				break;
			case '?':
				fprintf(stderr, "Warning: unrecognized option -%c\n", optopt);
				break;
			case ':':
				fprintf(stderr, "Error: missing argument for option -%c\n",
				        optopt);
				return -__LINE__;
			default:
				fprintf(stderr, "Unexpected getopt return: '%c'\n", ch);
				break;
		}
	}
	if (opt_v_met && opt_r_met) {
		fputs("Error: options --update-rate and --vsync are incompatible\n",
		      stderr);
		return -__LINE__;
	}
	if (opt_b_met && opt_n_met) {
		fputs("Error: options --border-width and --no-border are"
		      " incompatible\n", stderr);
		return -__LINE__;
	}
	if (opt_f_met && (opt_i_met || opt_o_met)) {
		fputs("Error: options --file is incompatible with --input-file and"
		      " --output-file", stderr);
		return -__LINE__;
	}
	if (opt_i_met && (opt_w_met || opt_h_met)) {
		fputs("Error: options --width and --height are incompatible with"
		      " --input-file", stderr);
		return -__LINE__;
	}
	for (int i = optind; i < argc; ++i) {
		fprintf(stderr,
		        "Warning: skipping unrecognized non-option argument \"%s\"\n",
		        argv[i]);
	}
	return 0;
}


#ifndef _GNU_SOURCE

#include <string.h> /* for strcmp */



static int _locateLongOpt(const char *name, const struct option *longopts, int *argindex) {
	const char *eqsignindex = strchr(name, '=');
	if (eqsignindex == NULL) {
		*argindex = -1;
	} else {/* The option contains an attached argument, make a copy of the option name only */
		int optlen = (int) (eqsignindex - name);
		*argindex = optlen + 3; /* + 2 for leading hyphens, + 1 for equals sign */
		char *nameonly = alloca(optlen);
		memcpy(nameonly, name, optlen);
		nameonly[optlen] = 0;
		name = nameonly;
	}
	for (int i = 0; longopts[i].name != NULL && longopts[i].name[0] != '\0'; ++i) {
		if (strcmp(name, longopts[i].name) == 0) {
			return i;
		}
	}
	return -1;
}

static int _handleLongOpt(char *token, char *const *argv, const struct option *longopts, int *longindex) {
	int argindex;
	int longoptindex = _locateLongOpt(&token[2], longopts, &argindex);
	if (longindex != NULL) {
		*longindex = longoptindex;
	}
	++optind;
	if (longoptindex < 0) { /* Long option not found */
		optopt = '?';
		return '?';
	}
	char optchar = longopts[longoptindex].val;
	if (longopts[longoptindex].arg_type == no_argument) {
		if (argindex > 0) { /* Unexpected argument */
			optopt = optchar;
			return '?';
		}
		return optchar;
	}
	if (argindex > 0) {
		optarg = &token[argindex];
		return optchar;
	}
	optarg = argv[optind];
	++optind;
	return optchar;
}

static int _optArgType(char opt, const char *optstring, enum arg_type *arg_type) {
	for (; *optstring != '\0'; ++optstring) {
		if (*optstring == opt) {
			*arg_type = *(optstring + 1) == ':' ? required_argument : no_argument;
			return 0;
		}
	}
	return -1;
}

static int nextchar = 0; /* The index to the next char option, useful when several are attached */
int getopt_long(int argc, char *const argv[], const char *optstring,
                const struct option *longopts, int *longindex) {
	char *token;
	if (argc == 1 || optind >= argc || (token = argv[optind]) == NULL) {
		return -1; /* End of command-line */
	}
	if (nextchar == 0) {
		if (token[0] == '\0' || token[0] != '-' || token[1] == '\0' || token[1] == '-' && token[2] == '\0') {
			return -1; /* Met a non-option argument (notably an empty string or a single -) or a -- */
		}
		char optchar = token[1];
		if (optchar == '-') { /* Two leading hyphens: long option */
			nextchar = 0;
			return _handleLongOpt(token, argv, longopts, longindex);
		} else {
			nextchar = 1;
		}
	}
	enum arg_type arg_type;
	char optchar = token[nextchar];
	if (_optArgType(optchar, optstring, &arg_type) < 0) {
		optopt = optchar;
		return '?';
	}
	if (arg_type == no_argument) {
		if (token[nextchar + 1] == '\0') { /* End of current command-line token, prepare pointers to next */
			nextchar = 0;
			++optind;
		} else {
			++nextchar;
		}
		return optchar;
	}
	if (token[nextchar + 1] != '\0') { /* Option's argument is attached to the option */
		optarg = &token[nextchar + 1];
		nextchar = 0;
		++optind;
		return optchar;
	}
	/* Option's argument is next command-line token */
	if (++optind > argc) { /* but there is none after the current one */
		optarg = NULL;
		optopt = optchar;
		return optstring[0] == ':' ? ':' : '?';
	}
	optarg = argv[optind];
	nextchar = 0;
	++optind; /* Increment once more to point to the next option */
	return optchar;
}

#endif
