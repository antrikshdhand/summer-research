#ifndef SAMPLE_H
#define SAMPLE_H

#include "../../fuzzer/src/c/grammar.h"
#include "hash_table.h"

#define EMPTY_TOKEN 0x88

int key_get_num_strings(Token token, Grammar* grammar, size_t l_str);
int rule_get_num_strings(Rule rule, Grammar* grammar, size_t l_str);

#endif