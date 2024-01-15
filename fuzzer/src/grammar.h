#include <stdint.h>

#ifndef GRAMMAR_H
#define GRAMMAR_H

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

typedef struct TokenArray {
    size_t index;
    Token tokens[MAX_STRINGS_IN_LANGUAGE];
} TokenArray;

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

// Note that the NonTerminals are listed in ascending order. This is important
// to allow for O(1) lookup of index in the fuzzing algorithm.
Grammar sample_grammar = {
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

void unify_key_inv(Token key, Grammar* grammar, TokenArray* fuzzed);
void unify_rule_inv(Rule rule, Grammar* grammar, TokenArray* fuzzed);

#endif