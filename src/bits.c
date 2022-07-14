#include "bits.h"

#include <string.h> /* for memcpy */



void copy_bits(const char *restrict src, size_t src_offset, char *restrict dest,
               size_t dest_offset, size_t length) {
	/* Skip the full bytes until the first byte that is partially used */
	src += src_offset >> 3;
	dest += dest_offset >> 3;
	src_offset &= 7;
	dest_offset &= 7;
	if (src_offset == dest_offset) {
		/* Source and dest share the bit alignment inside bytes: an optimization
		   is possible by writing the partial first byte, blitting all the full
		   bytes in a single memcpy call, and filling the final partial byte */
		if (src_offset != 0) {
			/* There is a partial byte offset: copy the first byte, masking out
			   the bits not concerned */
			char mask = 0xff >> src_offset;
			dest[0] = src[0] & mask;
			/* We increment the pointers to skip the leading byte */
			++src;
			++dest;
		}
		size_t length_bytes = (length - src_offset) >> 3;
		if (length_bytes > 0) {
			memcpy(dest, src, length_bytes);
		}
		length = (length - src_offset) & 7;
		if (length != 0) {
			/* There is a trailing partial byte, copy it */
			char mask = 0xff << (8 - length);
			dest[length_bytes] = src[length_bytes] & mask;
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
