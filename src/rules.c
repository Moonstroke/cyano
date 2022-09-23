#include "rules.h"


#include <stdbool.h>
#include <stddef.h> /* for NULL */
#include <string.h> /* for strcmp */


#define NB_NAMES 32



static const char *names[] = {
	"2x2",
	"34 Life",
	"Amoeba",
	"Assimilation",
	"Coagulations",
	"Conway's Life",
	"Coral",
	"Corrosion of Conformity",
	"Day & Night",
	"Diamoeba",
	"Flakes",
	"Gnarl",
	"HighLife",
	"Highlife",
	"InverseLife",
	"Inverse life",
	"Life 3-4",
	"Life without Death",
	"Long Life",
	"Long life",
	"LwoD",
	"Maze",
	"Mazectric",
	"Move",
	"Original",
	"Pseudo Life",
	"Pseudo life",
	"Replicator",
	"Seeds",
	"Serviettes",
	"Stains",
	"WalledCities"
};

static const char *rules[] = {
	"B36/S125",
	"B34/S34",
	"B357/S1358",
	"B345/S4567",
	"B378/S235678",
	"B3/S23",
	"B3/S45678",
	"B3/S124",
	"B3678/S34678",
	"B35678/S5678",
	"B3/S012345678",
	"B1/S1",
	"B36/S23",
	"B36/S23",
	"B0123478/S34678",
	"B0123478/S34678",
	"B34/S34",
	"B3/S012345678",
	"B345/S5",
	"B345/S5",
	"B3/S012345678",
	"B3/S12345",
	"B3/S1234",
	"B368/S245",
	"B3/S23",
	"B357/S238",
	"B357/S238",
	"B1357/S1357",
	"B2/S",
	"B234/S",
	"B3678/S235678",
	"B45678/S2345"
};


const char *get_rule_from_name(const char *name) {
	unsigned int i = 0;
	unsigned int start = 0;
	unsigned int end = NB_NAMES;
	while (!i && (end - start) > 1) {
		unsigned int middle = (start + end) / 2;
		int d = strcmp(name, names[middle]);
		if (d < 0) end = middle;
		else if (d > 0) start = middle;
		else i = middle;
	}
	return i ? rules[i] : NULL;
}


static const char *_handle_rule_digits(const char *rulestr, char *rule,
                                       bool current_cell_state) {
	/* The code below handles a single digit in a rulestring. It sets to 1 all
	   bits in rule that match the state given by the digit and the current
	   cell state boolean.
	   A bit matches if, in the nine bits of its index in rule, the number of
	   bits set (excluding the center bit, which correspond to the current
	   state of the examined cell) is equal to the given digit.
	   The eight bits considered for the population count correspond to the
	   state of the eight neighbor cells.
	   The assignment of the bits for each digit is made by iterating over all
	   possible bit combinations. The iteration code itself will not be
	   detailed, as it is simple arithmetic loops. An interesting fact to note
	   is the symmetry of combinations, this allowed to code the blocks
	   handling digits 5, 6, 7 and 8 by copying the block for 3, 2, 1 or 0,
	   respectively, only changing the actual index in the SET_BIT call by
	   reversing the eight bits corresponding to the neighbor cells, and all
	   combinations will be covered.
	   */
	int s = current_cell_state << 4;
	if (*rulestr == '0') {
		SET_BIT(rule, s, true);
		++rulestr;
	}
	if (*rulestr == '1') {
		for (int i = 0; i < 4; ++i) {
			SET_BIT(rule, s + (1 << i), true);
		}
		for (int i = 5; i < 9; ++i) {
			SET_BIT(rule, s + (1 << i), true);
		}
		++rulestr;
	}
	if (*rulestr == '2') {
		for (int i = 1; i < 4; ++i) {
			for (int j = 0; j < i; ++j) {
				SET_BIT(rule, s + (1 << i) + (1 << j), true);
			}
		}
		for (int i = 5; i < 9; ++i) {
			for (int j = 0; j < 4; ++j) {
				SET_BIT(rule, s + (1 << i) + (1 << j), true);
			}
			/* Skipped for the first iteration of the i loop */
			for (int j = 5; j < i; ++j) {
				SET_BIT(rule, s + (1 << i) + (1 << j), true);
			}
		}
		++rulestr;
	}
	if (*rulestr == '3') {
		for (int i = 2; i < 4; ++i) {
			for (int j = 1; j < i; ++j) {
				for (int k = 0; k < j; ++k) {
					SET_BIT(rule, s + (1 << i) + (1 << j) + (1 << k), true);
				}
			}
		}
		for (int i = 5; i < 9; ++i) {
			for (int j = 1; j < 4; ++j) {
				for (int k = 0; k < j; ++k) {
					SET_BIT(rule, s + (1 << i) + (1 << j) + (1 << k), true);
				}
			}
			for (int j = 5; j < i; ++j) {
				for (int k = 0; k < 4; ++k) {
					SET_BIT(rule, s + (1 << i) + (1 << j) + (1 << k), true);
				}
				for (int k = 5; k < j; ++k) {
					SET_BIT(rule, s + (1 << i) + (1 << j) + (1 << k), true);
				}
			}
		}
		++rulestr;
	}
	if (*rulestr == '4') {
		SET_BIT(rule, s + 15, true); /* 15 = 0b000001111 */
		for (int i = 5; i < 9; ++i) {
			for (int j = 2; j < 4; ++j) {
				for (int k = 1; k < j; ++k) {
					for (int l = 0; l < k; ++l) {
						SET_BIT(rule, s + (1 << i) + (1 << j) + (1 << k)
						                + (1 << l), true);
					}
				}
			}
			for (int j = 5; j < i; ++j) {
				for (int k = 1; k < 4; ++k) {
					for (int l = 0; l < k; ++l) {
						SET_BIT(rule, s + (1 << i) + (1 << j) + (1 << k)
						                + (1 << l), true);
					}
				}
				for (int k = 5; k < j; ++k) {
					for (int l = 0; l < 4; ++l) {
						SET_BIT(rule, s + (1 << i) + (1 << j) + (1 << k)
						                + (1 << l), true);
					}
					for (int l = 5; l < k; ++l) {
						SET_BIT(rule, s + (1 << i) + (1 << j) + (1 << k)
						                + (1 << l), true);
					}
				}
			}
		}
		++rulestr;
	}
	if (*rulestr == '5') {
		/* Same as 3 but with neighbor bits inverted */
		for (int i = 2; i < 4; ++i) {
			for (int j = 1; j < i; ++j) {
				for (int k = 0; k < j; ++k) {
					/* 495 = 0b111101111 */
					SET_BIT(rule, 495 + s - (1 << i) - (1 << j) - (1 << k),
							true);
				}
			}
		}
		for (int i = 5; i < 9; ++i) {
			for (int j = 1; j < 4; ++j) {
				for (int k = 0; k < j; ++k) {
					SET_BIT(rule, 495 + s - (1 << i) - (1 << j) - (1 << k),
							true);
				}
			}
			for (int j = 5; j < i; ++j) {
				for (int k = 0; k < 4; ++k) {
					SET_BIT(rule, 495 + s - (1 << i) - (1 << j) - (1 << k),
							true);
				}
				for (int k = 5; k < j; ++k) {
					SET_BIT(rule, 495 + s - (1 << i) - (1 << j) - (1 << k),
							true);
				}
			}
		}
		++rulestr;
	}
	if (*rulestr == '6') {
		/* Same as 2 but with neighbor bits inverted */
		for (int i = 1; i < 4; ++i) {
			for (int j = 0; j < i; ++j) {
				SET_BIT(rule, 495 + s - (1 << i) - (1 << j), true);
			}
		}
		for (int i = 5; i < 9; ++i) {
			for (int j = 0; j < 4; ++j) {
				SET_BIT(rule, 495 + s - (1 << i) - (1 << j), true);
			}
			for (int j = 5; j < i; ++j) {
				SET_BIT(rule, 495 + s - (1 << i) - (1 << j), true);
			}
		}
		++rulestr;
	}
	if (*rulestr == '7') {
		/* Same as 1 but with neighbor bits inverted */
		for (int i = 0; i < 4; ++i) {
			SET_BIT(rule, 495 + s - (1 << i), true);
		}
		for (int i = 5; i < 9; ++i) {
			SET_BIT(rule, 495 + s - (1 << i), true);
		}
		++rulestr;
	}
	if (*rulestr == '8') {
		SET_BIT(rule, 495 + s, true);
		++rulestr;
	}
	if (*rulestr != '/' && *rulestr != 'S' && *rulestr != '\0') {
		return NULL; /* Invalid character */
	}
	return rulestr;
}

int compile_rulestring(const char *rulestring, char *rule) {
	if (*rulestring == 'B') {
		++rulestring;
	}
	if ((rulestring = _handle_rule_digits(rulestring, rule, false)) == NULL) {
		return -__LINE__;
	}
	if (*rulestring == '/') {
		++rulestring;
		if (*rulestring == 'S') {
			++rulestring;
		}
	} else if (*rulestring == 'S') {
		++rulestring;
	} else {
		/* Missing explicit separation between B and S parts, or invalid
		   character */
		return -__LINE__;
	}
	rulestring = _handle_rule_digits(rulestring, rule, true);
	if (rulestring == NULL || *rulestring != '\0') {
		return -__LINE__; /* Invalid character (not end of string) */
	}
	return 0;
}
