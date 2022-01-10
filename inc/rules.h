/**
 * \file "rules.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file defines a single function that converts a \e Life rule name
 *        to the corresponding Golly format rule string.
 */
#ifndef RULES_H
#define RULES_H

/**
 * Retrieve the Golly format rule string from its corresponding name.
 *
 * \param[in] name The name of the \e Life rule
 *
 * \return The Golly format string description, or \c NULL if not found.
 */
const char *getRuleFromName(const char *name);


#endif // RULES_H
