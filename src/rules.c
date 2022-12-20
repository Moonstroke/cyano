/* SPDX-License-Identifier: CECILL-2.1 */
#include "rules.h"


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
	unsigned int start = 0;
	unsigned int end = NB_NAMES;
	while (end - start > 1) {
		unsigned int middle = (start + end) / 2;
		int relative_pos = strcmp(name, names[middle]);
		if (relative_pos < 0) {
			end = middle;
		} else if (relative_pos > 0) {
			start = middle;
		} else {
			return rules[middle];
		}
	}
	return NULL;
}
