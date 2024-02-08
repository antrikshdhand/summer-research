#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <stdint.h>
#include <stdlib.h>

/******************************/
/*                            */
/*   Grammar Configuration    */
/*                            */
/******************************/

#define ITERATIONS 10
#define START_TOKEN 0x80
#define TO_PRINT

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

typedef struct TokenArray 
{
    size_t index;
    Token tokens[MAX_STRINGS_IN_LANGUAGE];
} TokenArray;

extern Grammar GRAMMAR;

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