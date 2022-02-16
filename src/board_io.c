#include "board.h"

#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc */

#include "bits.h" /* for SET_BIT */



static inline int initBoardFromRepr(struct board *board, const char *repr,
                                    bool wrap) {
	const char *itr = repr;
	for (; *itr && *itr != '\n'; ++itr);
	unsigned int width = itr - repr;
	unsigned int height = 1;
	/* Stepping width characters is faster, but assumes repr is well-formed */
	for (; *itr; itr += width) {
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
	int rc = initBoardFromRepr(board, repr, wrap);
	if (rc < 0) {
		return rc;
	}

	for (size_t i = 0; repr[i]; ++i) {
		if (repr[i] == '@') {
			SET_BIT(board->cells, i, true);
		} else if (repr[i] != '.' && repr[i] != '\n') {
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
			repr[j * board->h + i] = getBoardCell(board, i, j) ? '@' : '.';
		}
		repr[j + board->w] = '\n';
	}
	repr[(board->w + 1) * board->h - 1] = '\0';
	return repr;
}
