#ifndef FUZZER_H 
#define FUZZER_H 

#include "grammar.h"

/**
 * @brief Performs inverse unification on a given key in a grammar. 
 * 
 * The expanded code is provided below for readability.
 * 
 * \code{.c}
 * // Get the non-terminal related to the given key.
 * NonTerminal nt = grammar->non_terminals[nt_index];
 *
 * // Pick a random rule associated with the non-terminal.
 * size_t rand_rule_index = rand() % nt.num_rules;
 * Rule rand_rule = nt.rules[rand_rule_index];
 * 
 * // Generate some Tokens from that Rule.
 * unify_rule_inv(rand_rule, grammar, fuzzed);
 * \endcode
 * 
 * @param key
 * @param grammar
 * @param fuzzed
 */
void unify_key_inv(Token key, Grammar* grammar, TokenArray* fuzzed);

/**
 * @brief For each token of the given rule, generate some terminal strings by
 * running `unify_key_inv` on it.
 * 
 * @param rule A specific rule contained in a NonTerminal from which to generate strings.
 * @param grammar A Grammar struct representing a BNF grammar.
 * @param fuzzed A previously allocated array in which to store all fuzzed strings.
 */
void unify_rule_inv(Rule rule, Grammar* grammar, TokenArray* fuzzed);

#endif