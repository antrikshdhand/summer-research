#include <stdio.h>
#include <stdlib.h>
#include "grammar.h"

/**
 * @brief Tests if the given key is a non-terminal by checking if the MSB is
 * set. If so, the function returns the index of the non-terminal in the
 * Grammar `non_terminals` array.
 * 
 * @param key The key to be verified as non-terminal.
 * @param grammar The grammar struct representing a BNF grammar.
 * @return int The index of the non-terminal inside the grammar, otherwise -1.
 */
int is_non_terminal(Token key) 
{
    // Check if MSB is set.
    if ((key & 0x80) == 0x80) {
        // If the key is a non-terminal, the index will be the least
        // significant byte.
        return key & 0x0F;
    }
    return -1;
}

void unify_key_inv(Token key, Grammar* grammar, TokenArray* fuzzed)
{
    size_t nt_index;
    if ((nt_index = is_non_terminal(key)) != -1)
    {
        /*              EQUIVALENT CODE             */
        // // Get the non-terminal related to the given key.
        // NonTerminal nt = grammar->non_terminals[nt_index];

        // // Pick a random rule associated with the non-terminal.
        // size_t rand_rule_index = rand() % nt.num_rules;
        // Rule rand_rule = nt.rules[rand_rule_index];

        // // Generate some Tokens from that Rule.
        // unify_rule_inv(rand_rule, grammar, fuzzed);

        unify_rule_inv(
            grammar->non_terminals[nt_index].rules[
                rand() % grammar->non_terminals[nt_index].num_rules
            ],
            grammar,
            fuzzed
        );
    }
    else
    {
        // The only string which a terminal symbol can generate is 
        // the symbol itself.
        fuzzed->tokens[fuzzed->index] = key;
        fuzzed->index++;
    }
}

void unify_rule_inv(Rule rule, Grammar* grammar, TokenArray* fuzzed)
{
    // For each token of the given rule, generate some terminal strings 
    // by running `unify_key_inv` on it.
    for (size_t i = 0; i < rule.num_tokens; i++)
    {
        unify_key_inv(rule.tokens[i], grammar, fuzzed);
    }
}

void print_token_array(TokenArray* fuzzed) 
{
    for (size_t i = 0; i < fuzzed->index; i++)
    {
        printf("0x%x\n", fuzzed->tokens[i]);
    }
}

#define ITERATIONS 100000
int main() 
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        TokenArray fuzzed;
        fuzzed.index = 0;

        unify_key_inv(0x80, &sample_grammar, &fuzzed);
        // print_token_array(&fuzzed);
    }
    return 0;
}

