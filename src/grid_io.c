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
	char state = (*repr)[0];
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
	for (; repr[0]; ++repr) {
		switch (repr[0]) {
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
			case '#':
				repr = strchr(repr + 1, '\n');
				if (repr == NULL) {
					return -__LINE__;
				}
				break;
 			default:
				if (isspace(repr[0])) {
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

	while (repr[0] == '#') { /* Ignore pre-header comment lines */
		repr = strchr(repr + 1, '\n');
		if (repr == NULL) {
			return -__LINE__;
		}
		++repr;
	}
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

	do { /* Skip header and comments afterwards, if any */
		repr = strchr(repr + 1, '\n');
		if (repr == NULL) {
			return -__LINE__;
		}
		++repr;
	} while (repr[0] == '#');

	return _init_cells_from_rle(grid, repr);
}

static inline int _init_grid_from_plain(struct grid *grid, const char *repr,
                                        bool wrap) {
	const char *first_nl = strchr(repr, '\n');
	if (first_nl == NULL) {
		return -__LINE__;
	}
	/* If a line ends with CRLF, all must, otherwise the file is considered
	   ill-formed (this makes it easier to implement code accepting both line
	   endings). We could check for CRLF and decrement width here, but it would
	   require decrementing each line length in the while loop. Instead, we
	   just check that all line lengths are equal *including* the CR, and
	   decrement the width to account for CRs only once at the end. */
	bool is_crlf = *(first_nl - 1) == '\r';
	unsigned int width = (unsigned int) (first_nl - repr);
	unsigned int height = 1;
	const char *next_nl;
	while ((next_nl = strchr(repr, '\n')) != NULL) {
		if (next_nl - repr == width) {
			/* Ensure that the grid repr is rectangular */
			++height;
		} else if (repr[0] != '!') {
			/* Invalid line length and current line is not a comment */
			return -__LINE__;
		}
		repr = next_nl + 1;
	}
	return init_grid(grid, is_crlf ? width - 1 : width, height, wrap);
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

	while (repr[0] == '!') {
		repr = strchr(repr + 1, '\n');
		if (repr == NULL) {
			return -__LINE__;
		}
		++repr;
	}

	rc = _init_grid_from_plain(grid, repr, wrap);
	if (rc < 0) {
		return rc;
	}

	/* Memorized "on" character, to detect when both @ and O are mixed within a
	   same file. Blank means uninitialized, ! means warning already output */
	char on_char = ' ';
	for (size_t i = 0; repr[0]; ++repr) {
		if (repr[0] == '@') {
			SET_BIT(grid->cells, i, true);
			if (on_char == 'O') {
				fputs("Warning: mixed @ and O as \"on\" characters\n", stderr);
				on_char = '!';
			} else if (on_char == ' ') {
				on_char = '@';
			}
		} else if (repr[0] == 'O') {
			SET_BIT(grid->cells, i, true);
			if (on_char == '@') {
				fputs("Warning: mixed @ and O as \"on\" characters\n", stderr);
				on_char = '!';
			} else if (on_char == ' ') {
				on_char = 'O';
			}
		} else if (repr[0] == '\n') {
			if (repr[1] == '!') {
				repr = strchr(repr + 2, '\n');
				if (repr == NULL) { /* No more comments, reached end of file */
					break;
				}
			}
			continue;
		} else if (repr[0] == '\r') {
			fputs("ici\n", stderr);
			if (repr[1] == '\n') {
				if (repr[2] == '!') {
					repr = strchr(&repr[3], '\n');
					if (repr == NULL) { /* No more comments, reached end of file */
						break;
					}
				}
				++repr;
				continue;
			} else {
				return -__LINE__;
			}
		} else if (repr[0] != '.') {
			return -__LINE__;
		}
		++i;
	}
	return 0;
}


char *get_grid_repr(const struct grid *grid) {
	/* Additional height characters for newlines and null terminator */
	char *repr = malloc((grid->w + 1) * grid->h);
	if (repr == NULL) {
		return NULL;
	}
	char cell_repr[] = {
		[DEAD] = '.',
		[ALIVE] = '@'
	};
	for (unsigned int j = 0; j < grid->h; ++j) {
		for (unsigned int i = 0; i < grid->w; ++i) {
			repr[j * (grid->w + 1) + i] = cell_repr[get_grid_cell(grid, i, j)];
		}
		repr[j * (grid->w + 1) + grid->w] = '\n';
	}
	repr[(grid->w + 1) * grid->h - 1] = '\0';
	return repr;
}
