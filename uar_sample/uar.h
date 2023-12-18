#ifndef UAR_H
#define UAR_H

/*
Given a context-free grammar (CFG) defined in Backus-Naur Form (BNF), such as:

G = {
    <start>     :   <digits>
    <digits>    :   <digit> <digits> 
                    | <digit>
    <digit>     :   "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
},

which produces integers of variable length, we want to convert it into some
representation in C which can later be used for sampling.

We will use structs to represent our grammar, which is composed of
non-terminals and rules/productions:

- Our `Grammar` is made up of `NonTerminal`s.
- A `NonTerminal` struct captures the following form: A -> 𝛙. The LHS is the
    non-terminal itself, represented using angle brackets, and the RHS is the
    list of productions or rules of that non-terminal (we use `Rule` here for
    conciseness).
- A `Rule` is simply a string of characters which a non-terminal can produce.
*/

#define MAX_CHARACTERS_IN_STRING 10
#define MAX_RULES_FOR_NONTERMINAL 10
#define MAX_NONTERMINALS_IN_GRAMMAR 10

typedef struct Rule {
    const char* string[MAX_CHARACTERS_IN_STRING];
} Rule;

typedef struct NonTerminal {
    const char* name;
    Rule rules[MAX_RULES_FOR_NONTERMINAL];
} NonTerminal;

typedef struct Grammar {
    NonTerminal non_terminals[MAX_NONTERMINALS_IN_GRAMMAR];
} Grammar;


// Grammar struct
const Grammar grammar = { 
    // Array of non-terminals
    {
        // Non-terminal 1
        {
            // Name
            "<start>",

            // Array of rules, where each rule is itself an array of strings
            { 
                {"<digits>"}
            }
        },

        // Non-terminal 2
        {
            "<digits>",
            {
                {"<digit>", "<digits>"},
                {"<digit>"}
            }
        },

        // Non-terminal 3
        {
            "<digit>",
            {
                {"0"}, {"1"}, {"2"}, {"3"}, {"4"}, 
                {"5"}, {"6"}, {"7"}, {"8"}, {"9"}
            }
        }
    }
};

#endif