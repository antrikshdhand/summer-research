/**
 * Given a context-free grammar (CFG) defined in Backus-Naur Form (BNF), 
 * such as:
 * 
 * G = {
 *      <start>     ::=   <digits>
 *      <digits>    ::=   <digit> <digits>
 *                    |   <digit>
 *      <digit>     ::=   "0" | "1" | "2" | "3" | "4" 
 *                    |   "5" | "6" | "7" | "8" | "9"
 * },
 * 
 * which produces integers of variable length, we want to convert it into some
 * representation in C which can later be used for sampling.
 * 
 * We will use structs to represent our grammar which is composed of
 * non-terminals and rules/productions:
 * 
 * - Our `Grammar` is made up of `NonTerminal`s.
 * - A `NonTerminal` struct captures the following form: A -> 𝛙. The LHS is the
 *   non-terminal itself, represented using angle brackets, and the RHS is the
 *   list of productions or rules of that non-terminal (we use `Rule` here for
 *   conciseness).
 * - A `Rule` is simply a string of characters which a non-terminal can produce.
 * 
 * You can use the Python script `bnf_converter.py` to convert a text file
 * containing a grammar definition into the below format to allow for static
 * allocation.
 */

#ifndef GRAMMAR_H
#define GRAMMAR_H 

#define MAX_CHARACTERS_IN_TOKEN 20
#define MAX_TOKENS_IN_RULE 20
#define MAX_RULES_FOR_NONTERMINAL 20
#define MAX_NONTERMINALS_IN_GRAMMAR 20

typedef struct Token
{
    char string[MAX_CHARACTERS_IN_TOKEN];
} Token;

typedef struct TokenArray
{
    int num_tokens;
    Token* tokens;
} TokenArray;

typedef struct Rule
{
    int num_tokens;
    Token tokens[MAX_TOKENS_IN_RULE];
} Rule;

typedef struct NonTerminal
{
    const char *name;
    int num_rules;
    Rule rules[MAX_RULES_FOR_NONTERMINAL];
} NonTerminal;

typedef struct Grammar
{
    int num_non_terminals;
    NonTerminal non_terminals[MAX_NONTERMINALS_IN_GRAMMAR];
} Grammar;

// Sample grammar struct
const Grammar sample_grammar = {
    3,

    // Array of non-terminals
    {
        // Non-terminal 1
        {
            // Name
            "<start>",
            
            // Number of rules this production has
            1,

            // Array of rules
            {
                // Rule 1
                {
                    // Number of tokens this rule has
                    1, 

                    // Array of Tokens
                    {
                        // Token 1
                        "<digits>"
                    }
                }
            }
        },

        // Non-terminal 2
        {
            "<digits>",
            2,
            {
                {2, {"<digit>", "<digits>"}},
                {1, {"<digit>"}}
            }
        },

        // Non-terminal 3
        {
            "<digit>",
            10,
            {
                {1, {"0"}}, {1, {"1"}}, {1, {"2"}}, {1, {"3"}}, {1, {"4"}}, 
                {1, {"5"}}, {1, {"6"}}, {1, {"7"}}, {1, {"8"}}, {1, {"9"}}
            }
        }
    }
};

int is_non_terminal(Token key, Grammar grammar);
TokenArray* unify_key_inv(Token key, Grammar grammar);
TokenArray* unify_rule_inv(Rule rule, Grammar grammar);

/**
 *   Prints out a given `Grammar` struct in Backus-Naur Form.
 *
 *   @param grammar_p: a pointer to the Grammar struct to be pretty-printed
 */ 
void pretty_print_grammar(const Grammar *grammar_p)
{
    Grammar grammar = *grammar_p;

    // For pretty-printing purposes: find the max width of non-terminal names.
    int max_name_width = 0;
    for (int i = 0; i < MAX_NONTERMINALS_IN_GRAMMAR; i++)
    {
        NonTerminal non_terminal = grammar.non_terminals[i];
        if (non_terminal.name == NULL)
        {
            break;
        }

        int current_width = strlen(non_terminal.name);
        if (current_width > max_name_width)
        {
            max_name_width = current_width;
        }
    }

    // Now we print the grammar.
    for (int i = 0; i < grammar.num_non_terminals; i++)
    {
        NonTerminal non_terminal = grammar.non_terminals[i];
        printf("%-*s : ", max_name_width, non_terminal.name);

        int first_rule = 1;
        for (int j = 0; j < non_terminal.num_rules; j++)
        {
            Rule rule = non_terminal.rules[j];

            if (!first_rule)
            {
                printf("%-*s ", max_name_width, "");
                printf("| ");
            }

            // Loop through and print each non-NULL string in the rule
            for (int k = 0; k < rule.num_tokens; k++)
            {
                printf("%s ", rule.tokens[k].string);
            }

            printf("\n");
            first_rule = 0;
        }
    }
}

#endif