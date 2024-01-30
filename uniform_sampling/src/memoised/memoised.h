#ifndef MEMOISED_H
#define MEMOISED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../fuzzer/src/c/grammar.h"

// Use the print_%_hash_table functions to adjust empirically.
#define GRAMMAR_TABLE_SIZE 20
#define KEY_TABLE_SIZE 60 
#define RULE_TABLE_SIZE 10 

#define LENGTH_NA -1
#define EMPTY_TOKEN 0x88

// We hash these to retrieve an index.
typedef struct KeyLengthTuple
{
    Token key;
    size_t l_str;
} KeyLengthTuple;

// What we are storing in the HashMaps.
typedef struct KeyNode KeyNode;
typedef struct RuleNode RuleNode;

struct KeyNode
{
    Token token; // 8-bit
    size_t l_str; // length of the string which the 8-bit token represents
    int count;
    RuleNode* rules;
    struct KeyNode* next;
};

struct RuleNode
{
    struct KeyNode* key;
    RuleNode* tail;
    size_t l_str;
    int count;
    struct RuleNode* next;
};

typedef struct TokenStr
{
    Token key;
    char* str;
    size_t strlen;
    struct TokenStr* next; // External chaining.
} TokenStr;

void print_key_node(KeyNode* kn)
{
    if (kn == NULL) 
    {
        printf("NULL\n");
        return;
    }
    printf("key: 0x%x, l_str: %lu, count: %d\n", kn->token, kn->l_str, kn->count);
}

void print_rule_node(RuleNode* rn)
{
    if (rn == NULL)
    {
        printf("NULL\n");
        return;
    }

    printf("key: 0x%x, l_str: %lu, count: %d\n", rn->key->token, rn->l_str, rn->count);

}

// Hash table is an array of pointers to structs.
typedef KeyNode* KeyHashTable[KEY_TABLE_SIZE];
typedef RuleNode* RuleHashTable[RULE_TABLE_SIZE];
typedef TokenStr* GrammarHashTable[GRAMMAR_TABLE_SIZE];

#endif