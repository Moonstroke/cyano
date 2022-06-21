#include "grid.h"

#include <ctype.h> /* for isspace */
#include <stddef.h> /* for size_t */
#include <stdio.h> /* for sscanf */
#include <stdlib.h> /* for malloc */
#include <string.h> /* for strlen, strcpy */

#include "bits.h" /* for SET_BIT */



static inline int _set_run_length(struct grid *grid, unsigned int *i,
                                  unsigned int j, const char **repr) {
	char *end = NULL;
	long length = strtol(*repr, &end, 10);
	*repr = end;
	if (*i + length > grid->w) {
		return -__LINE__;
	}
	char state = **repr;
	if (state == 'o') {
		for (int n = 0; n < length; ++n) {
			SET_BIT(grid->cells, j * grid->w + *i + n, true);
		}
	} else if (state != 'b') { /* Invalid character */
		return -__LINE__;
	}
	*i += length;
	return 0;
}

static inline int _init_cells_from_rle(struct grid *grid, const char *repr) {
	unsigned int i = 0;
	unsigned int j = 0;
	int rc;
	for (; *repr; ++repr) {
		switch (*repr) {
			case '!': /* End of repr */
				return 0;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if ((rc = _set_run_length(grid, &i, j, &repr)) < 0) {
					return rc;
				}
				break;
			case 'o':
				SET_BIT(grid->cells, j * grid->w + i, true);
			/* Fall-through intended */
			case 'b':
				if (++i > grid->w) {
					return -__LINE__;
				}
				break;
			case '$':
				/* No grid width checking because end of row can be omitted if
				   all cells are blank */
				if (++j >= grid->h) {
					return -__LINE__;
				}
				i = 0;
				break;
 			default:
				if (isspace(*repr)) {
					/* Whitespace is ignored anywhere outside of run length
					   specifications to allow for line wrapping and pattern
					   readability */
					break;
				}
				/* Unexpected character, may be a 0 starting a run length, NUL
				   byte, anything that is not handled in the cases is considered
				   invalid. */
				return -__LINE__;
		}
	}
	return 0;
}

static inline int _init_grid_from_rle(struct grid *grid, const char *repr,
                                      bool wrap) {
	char rule_buffer[22] = {0};
	unsigned int w, h;
	int rc = sscanf(repr, "x = %u, y = %u, rule = %22s", &w, &h, rule_buffer);
	if (rc < 2) {
		/* No proper RLE header line, probably not RLE at all */
		return -__LINE__;
	}
	/* x and y specifications are mandatory, rule is optional */
	bool add_rule = rc == 3;
	rc = init_grid(grid, w, h, wrap);
	if (rc < 0) {
		return rc;
	}
	if (add_rule) {
		size_t len = strlen(rule_buffer);
		char *rule = malloc(len);
		if (rule == NULL) {
			free_grid(grid);
			return -__LINE__;
		}
		strcpy(rule, rule_buffer);
		grid->rule = rule;
	}
	return _init_cells_from_rle(grid, strchr(repr, '\n') + 1);
}

static inline int _init_grid_from_plain(struct grid *grid, const char *repr,
                                        bool wrap) {
	const char *itr = repr;
	for (; *itr && *itr != '\n'; ++itr);
	unsigned int width = itr - repr;
	unsigned int height = 1;
	/* Stepping width + 1 characters is faster, but assumes repr is
	   well-formed */
	for (; *itr; itr += width + 1) {
		if (*itr == '\n') {
			++height;
		} else {
			/* We did not fall on a line break: repr is ill-formed */
			return -__LINE__;
		}
	}
	return init_grid(grid, width, height, wrap);
}

int load_grid(struct grid *grid, const char *repr, enum grid_format format,
              bool wrap) {
	int rc;
	if (format == GRID_FORMAT_RLE || format == GRID_FORMAT_UNKNOWN) {
		rc = _init_grid_from_rle(grid, repr, wrap);
		if (rc <= 0) { /* > 0 means not RLE */
			return rc;
		} else if (format == GRID_FORMAT_RLE) {
			return -__LINE__;
		}
	}
	rc = _init_grid_from_plain(grid, repr, wrap);
	if (rc < 0) {
		return rc;
	}

	for (size_t i = 0; *repr; ++i, ++repr) {
		if (*repr == '@') {
			SET_BIT(grid->cells, i, true);
		} else if (*repr == '\n') {
			++repr;
		} else if (*repr != '.') {
			return -__LINE__;
		}
	}
	return 0;
}


char *get_grid_repr(const struct grid *grid) {
	/* Additional height characters for newlines and null terminator */
	char *repr = malloc((grid->w + 1) * grid->h);
	if (repr == NULL) {
		return NULL;
	}
	for (unsigned int j = 0; j < grid->h; ++j) {
		for (unsigned int i = 0; i < grid->w; ++i) {
			repr[j * (grid->w + 1) + i] = get_grid_gell(grid, i, j) ? '@'
			                                                      : '.';
		}
		repr[j * (grid->w + 1) + grid->w] = '\n';
	}
	repr[(grid->w + 1) * grid->h - 1] = '\0';
	return repr;
}
