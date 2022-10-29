/**
 * \file "bits.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file defines a set of macros and functions to operate on a
 *        \c char array as a bit array.
 */
#ifndef BITS_H
#define BITS_H


#include <stddef.h> /* for size_t */
#include <stdio.h> /* for FILE */



/**
 *  Give the number of chars needed to store the given number of bits.
 *
 * \param[in] size The number of bits
 *
 * \return The minimal number of chars (octets) necessary to hold \p size bits
 */
inline size_t num_octets(size_t size) {
     size_t size_octets = size >> 3;
     if ((size & 7) != 0) {
          ++size_octets;
     }
     return size_octets;
}

/** Access the bit at specified index in the given bit array.
 *
 * \param[in] arr The array
 * \param[in] i   The index
 *
 * \return The value of the <tt>i</tt>-th bit in \p arr
 */
inline int get_bit(const char *arr, size_t i) {
     return (arr[i >> 3] >> (7 - (i & 7)) & 1);
}

/**
 * Assign the given value to the bit at the specified index in the array.
 *
 * \param[out] arr The array to modify
 * \param[in]  i   The index
 * \param[in]  val The value to assign
 */
inline void set_bit(char *arr, size_t i, int val) {
     if (val == 0) {
          arr[i >> 3] &= ~(1 << (7 - (i & 7)));
     } else {
          arr[i >> 3] |= 1 << (7 - (i & 7));
     }
}

/**
 * Invert the value of the bit at specified index in the given bit array.
 *
 * \param[out] arr The array
 * \param[in]  i   The index
 */
inline void toggle_bit(char *arr, size_t i) {
     arr[i >> 3] ^= 1 << (7 - (i & 7));
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
