/**
 * \file "mathutils.h"
 * \author joH1
 *
 * \version 1.0
 *
 * \brief A set of math-related or abstract computation functions.
 */
#ifndef MATHUTILS_H
#define MATHUTILS_H



/**
 * Return the arithmetic (positive) modulo of \p a and \p b.
 *
 * \param[in] a,b The two values
 *
 * \return The remainder of the division of \p a by \p b, between \c 0
 *         (inclusive) and \p b (exclusive)
 */
inline unsigned int pos_mod(int a, int b) {
	/* Only one of the loops will be entered. For most use cases in the
	   project, the loop entered will be iterated only once, so these are
	   almost if blocks */
	while (a < 0) {
		a += b;
	}
	while (a > b) {
		a -= b;
	}
	return a;
}


/**
 * Return the lesser of the two values.
 *
 * Defined as a macro to work with all integer types in a single declaration,
 * however all the usual macro caveats apply.
 *
 * @param a,b The two values to compare
 *
 * @return \p a if it is less than \p b, otherwise \p b
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))


#endif /* MATHUTILS_H */
