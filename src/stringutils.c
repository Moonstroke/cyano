#include "stringutils.h"


#include <stddef.h> /* for size_t */
#define _POSIX_C_SOURCE 200809L /* to enable strnlen in string.h */
#include <string.h> /* for strlen, strnlen, memcmp */



bool endswith(const char *string, const char *suffix) {
	size_t l1 = strlen(string);
	size_t l2 = strnlen(suffix, l1 + 1);
	return l1 >= l2 && memcmp(&string[l1 - l2], suffix, l2) == 0;
}
