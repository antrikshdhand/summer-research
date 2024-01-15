#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grammar.h"

// #define DEBUG 1
#define ITERATIONS 10000

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
    #ifdef DEBUG
    printf("\nKey: 0x%x\n", key);
    #endif

    size_t nt_index;
    if ((nt_index = is_non_terminal(key)) != -1)
    {
        // Get the non-terminal related to the given key.
        NonTerminal nt = grammar->non_terminals[nt_index];

        // Pick a random rule associated with the non-terminal.
        size_t rand_rule_index = rand() % nt.num_rules;
        Rule rand_rule = nt.rules[rand_rule_index];

        #ifdef DEBUG
        printf("It's non-terminal.\n");
        printf("rand_rule_index: %zu\n", rand_rule_index);
        printf("Random rule num tokens: %zu\n", rand_rule.num_tokens);
        printf("Random rule 0th token: 0x%x\n", rand_rule.tokens[0]);
        #endif

        // Generate some Tokens from that Rule.
        unify_rule_inv(rand_rule, grammar, fuzzed);
    }
    else
    {
        #ifdef DEBUG
        printf("It's a terminal symbol.\n");
        #endif

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
        Token token = rule.tokens[i];
        unify_key_inv(token, grammar, fuzzed);
    }
}

void print_token_array(TokenArray* fuzzed) 
{
    for (size_t i = 0; i < fuzzed->index; i++)
    {
        printf("0x%x\n", fuzzed->tokens[i]);
    }
}

void print_time_to_file(double cpu_time_used, int iterations)
{
    FILE* filep = fopen("out/c_result.txt", "w");
    if (!filep) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    fprintf(filep, "%d\n", iterations);
    fprintf(filep, "%f", cpu_time_used);
    fclose(filep);
}

int main() 
{
    // Ensures randomness when selecting rule in non-terminal
    srand(time(0));

    /* MEASURE ALGORITHM TIME */
    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock();

    for (int i = 0; i < ITERATIONS; i++)
    {
        TokenArray fuzzed;
        fuzzed.index = 0;

        Token start_token = 0x80;
        unify_key_inv(start_token, &sample_grammar, &fuzzed);
        // print_token_array(&fuzzed);
    }

    end_time = clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    print_time_to_file(cpu_time_used, ITERATIONS);

    return 0;
}

