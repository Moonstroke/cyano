#include "bits.h"

#include <string.h> /* for memcpy */



void copy_bits(const char *RESTRICT src, size_t src_offset, char *RESTRICT dest,
               size_t dest_offset, size_t length) {
	/* Skip the full bytes until the first byte that is partially used */
	src += src_offset >> 3;
	dest += dest_offset >> 3;
	src_offset &= 7;
	dest_offset &= 7;
	if (src_offset == dest_offset) {
		if (src_offset > 0) {
			/* Source and dest share the bit alignment inside bytes: an
			   optimization is possible by writing the partial first byte,
			   blitting all the full bytes in a single memcpy call, and filling
			   the final partial byte */
			for (size_t i = src_offset; i < 8; ++i) {
				SET_BIT(dest, i, GET_BIT(src, i));
			}
			/* Increment the pointers to skip the byte from which we just
			   copied the leading offset */
			++src;
			++dest;
		}
		size_t length_bytes = (length - src_offset) >> 3;
		memcpy(dest, src, length_bytes);
		for (size_t i = 0; i < (length & 7); ++i) {
			SET_BIT(dest + length_bytes, i, GET_BIT(src + length_bytes, i));
		}
	} else {
		/* No common alignment to leverage, fallback to naïve linear copy */
		for (size_t i = 0; i < length; ++i) {
			SET_BIT(dest, dest_offset + i, GET_BIT(src, src_offset + i));
		}
	}
}


void print_bits(const char *bits, size_t offset, size_t size, FILE *file) {
	for (size_t i = 0; i < size; ++i) {
		fputc('0' + GET_BIT(bits, offset + i), file);
	}
	fputc('\n', file);
}
