/**
 * A C implementation of "The simplest grammar fuzzer in the world"
 * https://rahul.gopinath.org/post/2019/05/28/simplefuzzer-01/
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "grammar.h"

#define DEBUG 1

int is_non_terminal(Token key, Grammar grammar);
TokenArray* unify_key_inv(Token key, Grammar grammar);
TokenArray* unify_rule_inv(Rule rule, Grammar grammar);

/**
 *   Checks if a given key exists in the list of non-terminals 
 *   of a given grammar.
 * 
 *   @param key: the string to be verified as non-terminal
 *   @param grammar: the Grammar struct representing a BNF grammar
 *   @return The index of the non-terminal inside the grammar, otherwise -1
 */
int is_non_terminal(Token key, Grammar grammar)
{
    int key_index = -1;
    for (int i = 0; i < grammar.num_non_terminals; i++)
    {
        NonTerminal nt = grammar.non_terminals[i];
        if (strncmp(key.string, nt.name, strlen(nt.name)) == 0)
        {
            key_index = i;
            break;
        }
    }

    return key_index;
}

/**
 * Append the contents of src to the end of dest.
 * 
 * @param src The TokenArray containing Tokens to be appended
 * @param dest The TokenArray which Tokens will be appended to
 * @return TokenArray* A pointer to the destination TokenArray
 */
TokenArray* append_token_arrays(TokenArray* src, TokenArray* dest)
{

    // Make space in dest memory for contents of src
    if (dest->tokens == NULL)
    {
        dest->tokens = malloc(src->num_tokens * sizeof(Token));
    }
    else
    {
        int new_size = dest->num_tokens + src->num_tokens;
        dest->tokens = realloc(
            dest->tokens, 
            (new_size) * sizeof(Token)
        );
    }

    // Copy all the strings from src to dest
    for (int j = 0; j < src->num_tokens; j++)
    {
        int index = j + dest->num_tokens;
        strcpy(
            dest->tokens[index].string,
            src->tokens[j].string
        );
    }

    dest->num_tokens += src->num_tokens;

    return dest;
}

/**
 * 
 *   Performs inverse unification on a given key in a grammar.
 *   i.e. The final result is a list of strings representing different possible
 *   combinations of terminal symbols that could be produced from the specified
 *   non-terminal key in the grammar.
 * 
 *   @param key: the string which to perform inverse unification on
 *   @param grammar: the Grammar struct representing a BNF grammar
 * 
 */
TokenArray* unify_key_inv(Token key, Grammar grammar)
{
    #ifdef DEBUG
    printf("\nENTERED unify_key_inv\n");
    printf("Key: %s\n", key.string);
    #endif

    TokenArray* fuzzed_strings = malloc(sizeof(TokenArray));
    fuzzed_strings->num_tokens = 0;
    fuzzed_strings->tokens = NULL;

    int non_terminal_index;
    if ((non_terminal_index = is_non_terminal(key, grammar)) != -1)
    {
        #ifdef DEBUG
        printf("It's non-terminal\n");
        #endif

        // Pick a random rule associated with the non-terminal and
        // generate all the strings it can produce.
        NonTerminal nt = grammar.non_terminals[non_terminal_index];
        int rand_rule_index = rand() % nt.num_rules;
        Rule rand_rule = nt.rules[rand_rule_index];

        #ifdef DEBUG
        printf("Random rule index: %d\n", rand_rule_index);
        printf("First token of random rule: %s\n", rand_rule.tokens[0].string);
        #endif

        TokenArray* strs_from_rand_rule = unify_rule_inv(rand_rule, grammar);

        #ifdef DEBUG
        printf("Back in unify_key_inv!\n\n");
        #endif

        fuzzed_strings = append_token_arrays(strs_from_rand_rule, fuzzed_strings);

        free(strs_from_rand_rule);
        
        return fuzzed_strings;
    }
    else
    {
        #ifdef DEBUG
        printf("Terminal: %s\n", key.string);
        #endif

        // The only string which a terminal symbol can generate is 
        // the symbol itself.
        TokenArray* ta = malloc(sizeof(TokenArray));
        ta->num_tokens = 1;
        ta->tokens = malloc(1 * sizeof(Token));
        strcpy(ta->tokens[0].string, key.string);
        
        #ifdef DEBUG
        printf("String in return tokenArray: %s\n\n", ta->tokens[0].string);
        #endif

        return ta;
    }
}

TokenArray* unify_rule_inv(Rule rule, Grammar grammar)
{
    #ifdef DEBUG
    printf("\nENTERED unify_rule_inv\n");
    printf("Number of tokens in the rule: %d\n", rule.num_tokens);
    #endif

    // For each token of the given rule, see what terminal strings can
    // be produced by running `unify_key_inv` on it.
    TokenArray* fuzzed_strings = malloc(sizeof(TokenArray));
    fuzzed_strings->num_tokens = 0;
    fuzzed_strings->tokens = NULL;

    for (int i = 0; i < rule.num_tokens; i++)
    {
        Token token = rule.tokens[i];
        
        #ifdef DEBUG
        printf("i-th token about to be unified: %s\n", token.string);
        #endif

        TokenArray* unify = unify_key_inv(token, grammar);

        fuzzed_strings = append_token_arrays(unify, fuzzed_strings);

        #ifdef DEBUG
        printf("Back in unify_rule_inv!\n");
        if (i == rule.num_tokens - 1) {
            printf("About to return.\n");
        }
        #endif

    }

    #ifdef DEBUG
    printf("\n");
    #endif

    return fuzzed_strings;
}

void print_token_array(TokenArray* ta)
{
    for (int i = 0; i < ta->num_tokens; i++)
    {
        printf("%s\n", ta->tokens[i].string);
    }
}

int main() 
{
    srand(time(0));

    Grammar grammar = {
	2,
	{
		{
			"<cs61>",
			1,
			{
				{2, "cs61 is ", "<emotion>"}
			}
		},
		{
			"<emotion>",
			2,
			{
				{1, "awesome"},
				{1, "terrible"}
			}
		}
	}
    };
 

    Token start_token = {"<cs61>"};
    TokenArray* all_strings = unify_key_inv(start_token, grammar);
    print_token_array(all_strings);

    free(all_strings);

}