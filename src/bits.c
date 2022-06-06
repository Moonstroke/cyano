#include "bits.h"

#include <string.h> /* for memcpy */



void copyBits(const char *RESTRICT src, size_t srcOffset, char *RESTRICT dest,
              size_t destOffset, size_t length) {
	src += srcOffset >> 3;
	dest += destOffset >> 3;
	srcOffset &= 7;
	destOffset &= 7;
	if (((srcOffset - destOffset) & 7) == 0) {
		/* Source and dest share the bit alignment inside bytes: an optimization
		   is possible by writing the partial first byte, blitting all the full
		   bytes in a single memcpy call, and filling the final partial byte */
		for (size_t i = srcOffset; i < 8; ++i) {
			SET_BIT(dest, i, GET_BIT(src, i));
		}
		if (srcOffset > 0) {
			/* If we did write a leading offset to a byte, we increment the
			   pointers to skip it */
			++src;
			++dest;
		}
		size_t lengthBytes = (length - srcOffset) >> 3;
		memcpy(dest, src, lengthBytes);
		for (size_t i = 0; i < (length & 7); ++i) {
			SET_BIT(dest + lengthBytes, i, GET_BIT(src + lengthBytes, i));
		}
	} else {
		/* No usable shared alignment, fallback to naive linear copy */
		for (size_t i = 0; i < length; ++i) {
			SET_BIT(dest, destOffset + i, GET_BIT(src, srcOffset + i));
		}
	}
}


void printBits(const char *bits, size_t offset, size_t size, FILE *file) {
	for (size_t i = 0; i < size; ++i) {
		fputc('0' + GET_BIT(bits, offset + i), file);
	}
	fputc('\n', file);
}
