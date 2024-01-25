#include <stdio.h>
#include "grammar.h"
#include "fuzzer.h"

int is_non_terminal(Token key) 
{
    if ((key & 0x80) == 0x80) {
        return key & 0x0F;
    }
    return -1;
}

void unify_key_inv(Token key, Grammar* grammar, TokenArray* fuzzed)
{
    size_t nt_index;
    if ((nt_index = is_non_terminal(key)) != -1)
    {
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
    for (size_t i = 0; i < rule.num_tokens; i++)
    {
        unify_key_inv(rule.tokens[i], grammar, fuzzed);
    }
}

void print_token_array(TokenArray* fuzzed) 
{
    for (size_t i = 0; i < fuzzed->index; i++)
    {
        printf("0x%x ", fuzzed->tokens[i]);
    }
    printf("\n");
}