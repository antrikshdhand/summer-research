#ifndef FUZZER_H 
#define FUZZER_H 

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

/******************************/
/*                            */
/*   Grammar Configuration    */
/*                            */
/******************************/

#define ITERATIONS 10
#define START_TOKEN 0x80
// #define TO_PRINT

#define MAX_TOKENS_IN_RULE 1024 
#define MAX_RULES_FOR_NONTERMINAL 1024
#define MAX_NONTERMINALS_IN_GRAMMAR 128
#define MAX_STRINGS_IN_LANGUAGE 100

/**
 * A Grammar is an array of NonTerminals. 
 * A NonTerminal has a name (an 8-bit id), and an array of rules.
 * A Rule consists of several Tokens e.g. "<article> <noun>".
 * The Tokens are written in 8-bit form.
 */

typedef uint8_t Token;

typedef struct Rule
{
    size_t num_tokens;
    Token tokens[MAX_TOKENS_IN_RULE];
} Rule;

typedef struct NonTerminal
{
    Token name;
    size_t num_rules;
    Rule rules[MAX_RULES_FOR_NONTERMINAL];
} NonTerminal;

typedef struct Grammar
{
    size_t num_non_terminals;
    NonTerminal non_terminals[MAX_NONTERMINALS_IN_GRAMMAR];
} Grammar;

typedef struct TokenArray {
    size_t index;
    Token tokens[MAX_STRINGS_IN_LANGUAGE];
} TokenArray;

// Note that the NonTerminals are listed in ascending order. This is important
// to allow for O(1) lookup of index in the fuzzing algorithm.
Grammar GRAMMAR = {
    // Number of non-terminals
    6,

    // Array of NonTerminals
    {
        // NonTerminal 1
        {
            // Name of NonTerminal
            0x80,
            
            // Number of rules NonTerminal has
            1,

            // Array of Rules
            {
                // Rule 1
                {
                    // Number of Tokens in Rule
                    1, 

                    // Array of Tokens
                    {0x81}
                }
            }
        },
        // NonTerminal 2
        {
            0x81,
            1,
            {
                {
                    2,
                    {0x82, 0x83}
                }
            }
        },
        // NonTerminal 3
        {
            0x82,
            1,
            {
                {
                    2,
                    {0x84, 0x85}
                }
            }
        },
        // NonTerminal 4
        {
            0x83,
            3,
            {
                {
                    1,
                    {0x05}
                },
                {
                    1,
                    {0x06}
                },
                {
                    1,
                    {0x07}
                }
            }
        },
        // NonTerminal 5
        {
            0x84,
            2,
            {
                {
                    1,
                    {0x03}
                },
                {
                    1,
                    {0x04}
                }
            }
        },
        // NonTerminal 6
        {
            0x85,
            3,
            {
                {
                    1,
                    {0x00}
                },
                {
                    1,
                    {0x01}
                },
                {
                    1,
                    {0x02}
                }
            }
        }
    }
};

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

/**
 * @brief Tests if the given key is a non-terminal by checking if the MSB is
 * set. If so, the function returns the index of the non-terminal in the
 * Grammar `non_terminals` array. The index will be the LSB, given that the 
 * Grammar struct was created properly.
 * 
 * @param key The key to be verified as non-terminal.
 * @param grammar The grammar struct representing a BNF grammar.
 * @return int The index of the non-terminal inside the grammar, otherwise -1.
 */
int is_non_terminal(Token key);

#endif