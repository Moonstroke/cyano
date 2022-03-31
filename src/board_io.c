#include "board.h"

#include <stddef.h> /* for size_t */
#include <stdio.h> /* for sscanf */
#include <stdlib.h> /* for malloc */
#include <string.h> /* for strlen, strcpy */

#include "bits.h" /* for SET_BIT */



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
	// TODO init board cells
	return 0;
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
