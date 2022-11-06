#include "grid.h"

#include <ctype.h> /* for isspace */
#include <stddef.h> /* for size_t */
#include <stdio.h> /* for sscanf */
#include <stdlib.h> /* for malloc */
#include <string.h> /* for strlen, strcpy */

#include "bits.h" /* for SET_BIT */
#include "utils.h" /* for CHECK_NULL */



static inline int _set_run_length(struct grid *grid, unsigned int *row,
                                  unsigned int col, const char **repr) {
	char *end = NULL;
	long length = strtol(*repr, &end, 10);
	*repr = end;
	if (*row + length > grid->width) {
		return -__LINE__;
	}
	char state = (*repr)[0];
	if (state == 'o') {
		for (int n = 0; n < length; ++n) {
			set_bit(grid->cells, col * grid->width + *row + n, 1);
		}
	} else if (state != 'b') { /* Invalid character */
		return -__LINE__;
	}
	*row += length;
	return 0;
}

static inline int _init_cells_from_rle(struct grid *grid, const char *repr) {
	unsigned int row = 0;
	unsigned int col = 0;
	int rc;
	for (; repr[0] != '\0'; ++repr) {
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
				if ((rc = _set_run_length(grid, &row, col, &repr)) < 0) {
					return rc;
				}
				break;
			case 'o':
				set_bit(grid->cells, col * grid->width + row, 1);
			/* Fall-through intended */
			case 'b':
				if (++row > grid->width) {
					return -__LINE__;
				}
				break;
			case '$':
				/* No grid width checking because end of row can be omitted if
				   all cells are blank */
				if (++col >= grid->height) {
					return -__LINE__;
				}
				row = 0;
				break;
			case '#':
				CHECK_NULL(repr = strchr(repr + 1, '\n'));
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
	unsigned int width;
	unsigned int height;

	while (repr[0] == '#') { /* Ignore pre-header comment lines */
		CHECK_NULL(repr = strchr(repr + 1, '\n'));
		++repr;
	}
	/* grid->rule cannot be passed directly to sscanf, because it will be
	   cleared in init_grid */
	int rc = sscanf(repr, "x = %u, y = %u, rule = %22s", &width, &height,
	                rule_buffer);
	if (rc < 2) {
		/* No proper RLE header line, probably not RLE at all */
		return -__LINE__;
	}
	/* x and y specifications are mandatory, rule is optional */
	bool add_rule = rc == 3;
	rc = init_grid(grid, width, height, wrap);
	if (rc < 0) {
		return rc;
	}
	if (add_rule) {
		memcpy(grid->rule, rule_buffer, sizeof grid->rule);
	}

	do { /* Skip header and comments afterwards, if any */
		CHECK_NULL(repr = strchr(repr + 1, '\n'));
		++repr;
	} while (repr[0] == '#');

	return _init_cells_from_rle(grid, repr);
}

static inline int _init_grid_from_plain(struct grid *grid, const char *repr,
                                        bool wrap) {
	const char *first_lf = strchr(repr, '\n');
	/* If there is no next line (EOF hit, usually) or if there is a line break
	   on first pos, LF or CRLF (unexpected blank line), repr is malformed */
	if (first_lf == NULL
	    || first_lf == repr || (first_lf == repr + 1 && repr[0] == '\r')) {
		return -__LINE__;
	}
	/* If a line ends with CRLF, all must, otherwise the file is considered
	   ill-formed (this makes it easier to implement code accepting both line
	   endings). We could check for CRLF and decrement width here, but it would
	   require decrementing each line length in the while loop. Instead, we
	   just check that all line lengths are equal *including* the CR, and
	   decrement the width to account for CRs only once at the end. */
	bool is_crlf = *(first_lf - 1) == '\r';
	unsigned int width = (unsigned int) (first_lf - repr);
	unsigned int height = 1;
	const char *next_lf;
	while ((next_lf = strchr(repr, '\n')) != NULL) {
		if (next_lf - repr == width) {
			/* Ensure that the grid repr is rectangular */
			++height;
		} else if (repr[0] != '!') {
			/* Invalid line length and current line is not a comment */
			return -__LINE__;
		}
		repr = next_lf + 1;
	}
	return init_grid(grid, is_crlf ? width - 1 : width, height, wrap);
}

static void _check_alive_char(char *saved_alive_char, char alive_char,
                              char other_alive_char) {
	if (*saved_alive_char == other_alive_char) {
		fputs("Warning: mixed @ and O as \"on\" characters\n", stderr);
		*saved_alive_char = '!';
	} else if (*saved_alive_char == ' ') {
		*saved_alive_char = alive_char;
	}
}

static int _init_cells_from_plain(struct grid *grid, const char *repr) {
	/* Memorized "on" character, to detect when both @ and O are mixed within a
	   same file. Blank means uninitialized, ! means warning already output */
	char saved_alive_char = ' ';
	for (size_t bit_index = 0; repr[0] != '\0'; ++repr) {
		if (repr[0] == '@') {
			set_bit(grid->cells, bit_index, 1);
			_check_alive_char(&saved_alive_char, '@', 'O');
		} else if (repr[0] == 'O') {
			set_bit(grid->cells, bit_index, 1);
			_check_alive_char(&saved_alive_char, 'O', '@');
		} else if (repr[0] == '\n'
		           || (repr[0] == '\r' && (++repr)[0] == '\n')) {
			if (repr[1] == '!') {
				repr = strchr(&repr[2], '\n');
				if (repr == NULL) { /* No more comments, reached end of file */
					break;
				}
			}
			continue;
		} else if (repr[0] != '.') {
			return -__LINE__;
		}
		++bit_index;
	}
	return 0;
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
		CHECK_NULL(repr = strchr(repr + 1, '\n'));
		++repr;
	}

	rc = _init_grid_from_plain(grid, repr, wrap);
	if (rc < 0) {
		return rc;
	}

	return _init_cells_from_plain(grid, repr);
}


static inline char *_get_rle_header(const struct grid *grid,
                                    size_t *repr_start, size_t *allocated) {
	char header[64] = {0};
	int header_size = sprintf(header, "x = %u, y = %u, rule = %s\n",
	                          grid->width, grid->height, grid->rule);
	if (header_size < 0) {
		return NULL;
	}
	*allocated = header_size + (grid->width + 1) * grid->height + 1;
	char *repr = malloc(*allocated);
	if (repr == NULL) {
		return NULL;
	}
	strncpy(repr, header, header_size);
	*repr_start = header_size;
	return repr;
}

static inline char *_get_grid_rle(const struct grid *grid) {
	char cell_repr[] = {
		[DEAD] = 'b',
		[ALIVE] = 'o'
	};
	size_t repr_index = 0;
	size_t allocated;
	char *repr = _get_rle_header(grid, &repr_index, &allocated);
	if (repr == NULL) {
		return NULL;
	}
	for (unsigned int row = 0; row < grid->height; ++row) {
		unsigned int run_length;
		for (unsigned int col = 0; col < grid->width; col += run_length) {
			run_length = 1;
			enum cell_state run_state = get_grid_cell(grid, row, col);
			/* Get length of run: stop at either a different cell or the end
			   of the row */
			while (col + run_length < grid->width
			       && get_grid_cell(grid, row, col + run_length) == run_state) {
				++run_length;
			}
			/* Skip blank row endings (i.e. blank runs that reach the end of
			   a row) */
			if (run_state == DEAD && col + run_length == grid->width) {
				break;
			}
			if (run_length > 1) {
				int nb_written = sprintf(&repr[repr_index], "%u", run_length);
				if (nb_written <= 0) {
					free(repr);
					return NULL;
				}
				repr_index += nb_written;
			}
			repr[repr_index++] = cell_repr[run_state];
		}
		repr[repr_index++] = '$';
	}
	/* Overwrite last row terminator */
	repr[repr_index - 1] = '!';
	repr[repr_index] = '\0';
	if (repr_index + 1 < allocated) {
		repr = realloc(repr, repr_index + 1);
	}
	return repr;
}

static inline void _get_grid_plain(const struct grid *grid, char *repr) {
	char cell_repr[] = {
		[DEAD] = '.',
		[ALIVE] = '@'
	};
	for (unsigned int row = 0; row < grid->height; ++row) {
		for (unsigned int col = 0; col < grid->width; ++col) {
			repr[row * (grid->width + 1) + col] = cell_repr[get_grid_cell(grid, row, col)];
		}
		repr[row * (grid->width + 1) + grid->width] = '\n';
	}
	repr[(grid->width + 1) * grid->height - 1] = '\0';
}

char *get_grid_repr(const struct grid *grid, enum grid_format format) {
	char *repr;
	if (format == GRID_FORMAT_RLE) {
		repr = _get_grid_rle(grid);
	} else {
	/* Additional height characters for newlines and null terminator */
		repr = malloc((grid->width + 1) * grid->height);
		if (repr == NULL) {
			return NULL;
		}
		_get_grid_plain(grid, repr);
	}
	return repr;
}
