/**
 * \file "mathutils.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief A set of math-related or abstract computation functions.
 */
#ifndef MATHUTILS_H
#define MATHUTILS_H



/**
 * Returns the arithmetic (positive) modulo of \a a and \a b.
 *
 * \param[in] a The dividend
 * \param[in] b The divisor
 *
 * \return The remainder of the division of \a a by \a b, between \c 0
 *         inclusive and \a b (exclusive)
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


#endif /* MATHUTILS_H */
