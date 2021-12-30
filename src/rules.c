#include "rules.h"


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


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


const char *getRuleFromName(const char *n) {
	unsigned int i = 0, s = 0, e = NB_NAMES;
	while(!i && (e - s) > 1) {
		unsigned int m = (s + e) / 2;
		int d = strcmp(n, names[m]);
		if(d < 0) e = m;
		else if(d > 0) s = m;
		else i = m;
	}
	return i ? rules[i] : NULL;
}
