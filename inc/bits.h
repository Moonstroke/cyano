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



/**
 *  Give the number of chars needed to store the given number of bits.
 *
 * \param[in] size The number of bits
 *
 * \return The minimal number of chars (octets) necessary to hold \p size bits
 */
#define NUM_OCTETS(size) ((size) / 8 + (((size) % 8) != 0))

/** Access the bit at specified index in the given bit array.
 *
 * \param[in] arr The array
 * \param[in] i   The index
 *
 * \return The boolean value of the <tt>i</tt>-th bit in \p arr
 */
#define GET_BIT(arr, i) (((arr)[(i) / 8] >> ((i) % 8)) & 1)
/**
 * Assign the given value to the bit at the specified index in the array.
 *
 * \param[out] arr The array to modify
 * \param[in]  i   The index
 * \param[in]  val The value to assign
 */
#define SET_BIT(arr, i, val) if (val) ((arr)[(i) / 8] |= 1 << ((i) % 8)); \
                             else ((arr)[(i) / 8] &= ~(1 << ((i) % 8)))
/**
 * Invert the value of the bit at specified index in the given bit array.
 *
 * \param[out] arr The array
 * \param[in]  i   The index
 */
#define TOGGLE_BIT(arr, i) ((arr)[(i) / 8] ^= 1 << ((i) % 8))


/**
 * Copy a range of bits.
 *
 * \param[in]  src        The array to copy from
 * \param[in]  srcOffset  The start position in the source array
 * \param[out] dest       The array to copy to
 * \param[in]  destOffset The start position in the destination array
 * \param[in]  length     The number of bits to copy
 */
void copyBits(const char* src, size_t srcOffset, char *dest, size_t destOffset,
              size_t length);


#endif /* BITS_H */
