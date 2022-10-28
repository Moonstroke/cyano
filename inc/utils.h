/**
 * \file utils.h
 * \author joH1
 *
 * \version 0.1
 *
 * \brief A set of macros useful for clean and readable code.
 */
#ifndef UTILS_H
#define UTILS_H


/**
 * Ensure that the given expression is greater than or equal to zero. If it is
 * not, the macro exits from the caller function, returning a negative value.
 *
 * This macro is intended to be passed calls to function returning an \c int
 * whose value is negative on error, therefore checking that the call has
 * succeeded.
 *
 * \param[in] expr The expression to check
 */
#define CHECK_RC(expr) if ((expr) >= 0); else return -__LINE__

/**
 * Ensure that the gien expression does not evaluate to \c NULL. If it does,
 * the macro exits from the caller function by returning a negative value.
 *
 * This macro is intended to wrap function calls, alleviating the caller code
 * from a if block that adds no functional value.
 *
 * \param[in] expr The expression to check
 */
#define CHECK_NULL(expr) if ((expr) != NULL); else return -__LINE__

#endif /* UTILS_H */
