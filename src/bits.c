#include "bits.h"

#include <string.h> /* for memcpy */


void copyBits(const char* restrict src, size_t srcOffset, char *restrict dest,
              size_t destOffset, size_t length) {
	// TODO optimize: use memcpy when offsets fall on a char limit
	src += srcOffset / 8;
	dest += destOffset / 8;
	srcOffset %= 8;
	destOffset %= 8;
	if ((srcOffset - destOffset) % 8 == 0) {
		/* Source and dest share the bit alignment inside bytes: an optimization
		   is possible by writing the partial first byte, blitting all the full
		   bytes in a single memcpy call, and filling the final partial byte */
		for (size_t i = 0; i < srcOffset; ++i) {
			SET_BIT(dest, i, GET_BIT(src, i));
		}
		size_t lengthBytes = (length - srcOffset) / 8;
		memcpy(dest, src, lengthBytes);
		for (size_t i = lengthBytes; i < lengthBytes + length % 8; ++i) {
			SET_BIT(dest, i, GET_BIT(src, i));
		}
	} else {
		/* No usable shared alignment, fallback to naive linear copy */
		for (size_t i = 0; i < length; ++i) {
			SET_BIT(dest, destOffset + i, GET_BIT(src, srcOffset + i));
		}
	}
}
