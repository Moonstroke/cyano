#ifndef CLOP_H
#define CLOP_H

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>

static const char *const OPTSTRING = "b:c:h:nr:vw:";

static const struct option const LONGOPTS[] = {
	{"board-width",  required_argument, NULL, 'w'},
	{"board-height", required_argument, NULL, 'h'},
	{"border-width", required_argument, NULL, 'b'},
	{"cell-dim",     required_argument, NULL, 'c'},
	{"no-border",    no_argument,       NULL, 'n'},
	{"update-rate",  required_argument, NULL, 'r'},
	{"vsync",        no_argument      , NULL, 'v'},
	{"", 0, NULL, 0}
};

void setvars(unsigned int *board_width,
             unsigned int *board_height,
             unsigned int *cell_pixels,
             unsigned int *update_rate,
             unsigned int *border_width,
             bool *use_vsync);

bool getvals(const int argc, const char *const argv[], const char *optstr, const struct option longopt[]);

#endif // CLOP_H
