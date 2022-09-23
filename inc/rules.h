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
const char *get_rule_from_name(const char *name);


/**
 * \brief Compile the rule string into an array of future cell states.
 *
 * \param[in] rulestring The rule string to compile
 * \param[in] rule       The array of bits where to compile the rulestring
 *
 * \return \c 0 if the compilation succeeded, a negative value otherwise (e.g.
 *         the rulestring is invalid)
 */
int compile_rulestring(const char *rulestring, char *rule);


#endif /* RULES_H */
