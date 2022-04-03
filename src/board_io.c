#include "board.h"

#include <ctype.h> /* for isspace */
#include <stddef.h> /* for size_t */
#include <stdio.h> /* for sscanf */
#include <stdlib.h> /* for malloc */
#include <string.h> /* for strlen, strcpy */

#include "bits.h" /* for SET_BIT */



static inline int setRunLength(struct board *board, unsigned int *i,
                               unsigned int j, const char **repr) {
	char *end = NULL;
	long length = strtol(*repr, &end, 10);
	*repr = end;
	if (*i + length > board->w) {
		return -4;
	}
	char state = **repr;
	if (state == 'o') {
		for (int n = 0; n < length; ++n) {
			SET_BIT(board->cells, j * board->w + *i + n, true);
		}
	} else if (state != 'b') { /* Invalid character */
		return -5;
	}
	*i += length;
	return 0;
}

static inline int initCellsFromRLE(struct board *board, const char *repr) {
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
				if ((rc = setRunLength(board, &i, j, &repr)) < 0) {
					return rc;
				}
				break;
			case 'o':
				SET_BIT(board->cells, j * board->w + i, true);
			/* Fall-through intended */
			case 'b':
				if (++i > board->w) {
					return -1;
				}
				break;
			case '$':
				/* No board width checking because end of row can be omitted if
				   all cells are blank */
				if (++j >= board->h) {
					return -2;
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
				return -3;
		}
	}
	return 0;
}

static inline int initBoardFromRLE(struct board *board, const char *repr,
                                   bool wrap) {
	char rule_buffer[22] = {0};
	unsigned int w, h;
	int rc = sscanf(repr, "x = %u, y = %u, rule = %22s", &w, &h, rule_buffer);
	if (rc < 2) {
		/* No proper RLE header line, probably not RLE at all */
		return -2;
	}
	/* x and y specifications are mandatory, rule is optional */
	bool add_rules = rc == 3;
	rc = initBoard(board, w, h, wrap);
	if (rc < 0) {
		return rc;
	}
	if (add_rules) {
		size_t len = strlen(rule_buffer);
		char *rule = malloc(len);
		if (rule == NULL) {
			freeBoard(board);
			return -3;
		}
		strcpy(rule, rule_buffer);
		board->rules = rule;
	}
	return initCellsFromRLE(board, strchr(repr, '\n') + 1);
}

static inline int initBoardFromRepr(struct board *board, const char *repr,
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
			return -2;
		}
	}
	return initBoard(board, width, height, wrap);
}

int loadBoard(struct board *board, const char *repr, bool wrap) {
	int rc = initBoardFromRLE(board, repr, wrap);
	if (rc <= 0) { /* > 0 means not RLE */
		return rc;
	}
	rc = initBoardFromRepr(board, repr, wrap);
	if (rc < 0) {
		return rc;
	}

	for (size_t i = 0; *repr; ++i, ++repr) {
		if (*repr == '@') {
			SET_BIT(board->cells, i, true);
		} else if (*repr == '\n') {
			++repr;
		} else if (*repr != '.') {
			return -3;
		}
	}
	return 0;
}


char *getBoardRepr(const struct board *board) {
	/* Additional height characters for newlines and null terminator */
	char *repr = malloc((board->w + 1) * board->h);
	if (repr == NULL) {
		return NULL;
	}
	for (unsigned int j = 0; j < board->h; ++j) {
		for (unsigned int i = 0; i < board->w; ++i) {
			repr[j * (board->w + 1) + i] = getBoardCell(board, i, j) ? '@'
			                                                         : '.';
		}
		repr[j * (board->w + 1) + board->w] = '\n';
	}
	repr[(board->w + 1) * board->h - 1] = '\0';
	return repr;
}
