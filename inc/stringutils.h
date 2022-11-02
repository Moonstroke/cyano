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
 * \brief Checks whether the end of the string corresponds to the suffix.
 *
 * \param[in] string The string to test
 * \param[in] suffix The string to find at the end
 *
 * \return \c true iff the end of the string ends matches the suffix
 */
bool endswith(const char *string, const char *suffix);


#endif /* STRINGUTILS_H */
