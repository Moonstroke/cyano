/* SPDX-License-Identifier: CECILL-2.1 */
/**
 * \file "bits.h"
 * \author Joachim "Moonstroke" MARIE
 *
 * \version 1.0
 *
 * \brief This file defines a set of macros and functions to operate on a
 *        \c char array as a bit array.
 */
#ifndef BITS_H
#define BITS_H


#include <stddef.h> /* for size_t */
#include <stdio.h> /* for FILE */



/**
 *  Give the number of 8-bit bytes needed to store the given amount of bits.
 *
 * \param[in] size The number of bits
 *
 * \return The minimal number of chars (octets) necessary to hold \p size bits
 */
inline size_t num_octets(size_t size) {
	size_t size_octets = size / 8;
	if (size % 8 != 0) {
		++size_octets;
	}
	return size_octets;
}

/** Access the bit at specified index in the given bit array.
 *
 * \param[in] bits  The bit array
 * \param[in] index The index of the bit to retrieve
 *
 * \return The value of the <tt>index</tt>-th bit in \p bits
 */
inline int get_bit(const char *bits, size_t index) {
	return (bits[index / 8] >> (7 - index % 8) & 1);
}

/**
 * Assign the given value to the bit at the specified index in the array.
 *
 * \param[out] bits  The bit array
 * \param[in]  index The index of the bit to modify
 * \param[in]  value The value to assign
 */
inline void set_bit(char *bits, size_t index, int value) {
	if (value == 0) {
		bits[index / 8] &= ~(1 << (7 - index % 8));
	} else {
		bits[index / 8] |= 1 << (7 - index % 8);
	}
}

/**
 * Invert the value of the bit at specified index in the given bit array.
 *
 * \param[out] bits  The bit array
 * \param[in]  index The index of the bit to invert
 */
inline void toggle_bit(char *bits, size_t index) {
	bits[index / 8] ^= 1 << (7 - index % 8);
}


/**
 * Copy a range of bits.
 *
 * \param[in]  src         The array to copy from
 * \param[in]  src_offset  The start position in the source array
 * \param[out] dest        The array to copy to
 * \param[in]  dest_offset The start position in the destination array
 * \param[in]  length      The number of bits to copy
 *
 * \note The implementation assumes that the source and destination arrays do
 *       not overlap.
 */
void copy_bits(const char *src, size_t src_offset, char *dest,
               size_t dest_offset, size_t length);


/**
 * Print the bit array to the given file or stream. Mostly intended for
 * debugging purposes.
 *
 * \param[in] bits   The bit array
 * \param[in] offset The number of bits to skip from the start of the array
 * \param[in] size   The number of bits to print
 * \param[in] file   The file or stream to write to
 */
void print_bits(const char *bits, size_t offset, size_t size, FILE *file);


#endif /* BITS_H */
