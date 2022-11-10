/**
 * \file "rules.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file defines a single function that converts a rule name to the
 *        corresponding rulestring.
 */
#ifndef RULES_H
#define RULES_H



/**
 * Retrieve a rulestring from its corresponding rule name.
 *
 * \param[in] name The name of the rule
 *
 * \return The rulestring of given name, or \c NULL if not found.
 */
const char *get_rule_from_name(const char *name);


#endif /* RULES_H */
