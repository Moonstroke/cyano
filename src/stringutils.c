#include "stringutils.h"


#include <stddef.h> /* for size_t */
#define _POSIX_C_SOURCE 200809L /* to enable strnlen in string.h */
#include <string.h> /* for strlen, strnlen, memcmp */



bool endswith(const char *restrict s1, const char *restrict s2) {
	size_t l1 = strlen(s1);
	size_t l2 = strnlen(s2, l1 + 1);
	return l1 >= l2 && memcmp(&s1[l1 - l2], s2, l2) == 0;
}
