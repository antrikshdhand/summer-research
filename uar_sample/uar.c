#include <stdio.h>
#include <string.h>

#include "uar.h"

void pretty_print_grammar(const Grammar* grammar_p) {
    Grammar grammar = *grammar_p;

    // For pretty-printing purposes: find the max width of non-terminal names.
    int max_name_width = 0;
    for (int i = 0; i < MAX_NONTERMINALS_IN_GRAMMAR; i++) {
        NonTerminal non_terminal = grammar.non_terminals[i];
        if (non_terminal.name == NULL) {
            break;
        }

        int current_width = strlen(non_terminal.name);
        if (current_width > max_name_width) {
            max_name_width = current_width;
        }
    }

    // Now we print the grammar.
    for (int i = 0; i < MAX_NONTERMINALS_IN_GRAMMAR; i++) {
        NonTerminal non_terminal = grammar.non_terminals[i];
        if (non_terminal.name == NULL) {
            break;
        }
        printf("%-*s : ", max_name_width, non_terminal.name);

        int first_rule = 1;
        for (int j = 0; j < MAX_RULES_FOR_NONTERMINAL; j++) {
            Rule rule = non_terminal.rules[j];
            if (rule.string[0] == NULL) {
                break;
            }

            if (!first_rule) {
                printf("%-*s ", max_name_width, "");
                printf("| ");
            }

            // Loop through and print each non-NULL string in the rule
            for (int k = 0; rule.string[k] != NULL; k++) {
                printf("%s ", rule.string[k]);
            }

            printf("\n");
            first_rule = 0;
        }
    }
}

int main() {
    pretty_print_grammar(&grammar);

    return 0;
}