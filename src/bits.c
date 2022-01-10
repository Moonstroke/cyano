#include "bits.h"


void copyBits(const char* src, size_t srcOffset, char *dest, size_t destOffset,
              size_t length) {
	for (size_t i = 0; i < length; ++i) {
		SET_BIT(dest, destOffset + i, GET_BIT(src, srcOffset + i));
	}
}
