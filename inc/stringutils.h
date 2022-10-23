/**
 * \file "stringutils.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief Declarations of functions manipulating strings; an extension of the
 *        standard header string.h.
 */
#ifndef STRINGUTILS_H
#define STRINGUTILS_H


#include <stdbool.h>



/**
 * \brief Checks whether the end of the first string corresponds to the second
 *        one.
 *
 * \param[in] s1 The first string
 * \param[in] s2 The second string (the suffix)
 *
 * \return \c true iff the first string ends with the second
 */
bool endswith(const char *s1, const char *s2);


#endif /* STRINGUTILS_H */
