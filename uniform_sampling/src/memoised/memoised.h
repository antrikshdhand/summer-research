#ifndef MEMOISED_H
#define MEMOISED_H

#include <stdio.h>
#include <stdlib.h>
#include "../../../fuzzer/src/c/grammar.h"

#define MAX_TOKEN_LEN 50
#define TABLE_SIZE 25 

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
    void (*print_key_node)(KeyNode* kn);
};

struct RuleNode
{
    struct KeyNode* key;
    RuleNode* tail;
    size_t l_str;
    int count;
    struct RuleNode* next;
    void (*print_rule_node)(RuleNode* rn);
};

typedef struct TokenStr
{
    uint8_t key;
    char str[MAX_TOKEN_LEN];
    int strlen;
    struct TokenStr* next; // External chaining.
} TokenStr;

void print_key_node(KeyNode* kn)
{
    printf("key: 0x%x <%lu> count: %d\n", kn->token, kn->l_str, kn->count);
}

void print_rule_node(RuleNode* rn)
{
    printf("head: 0x%x tail: 0x%x <%lu> count: %d\n", rn->key->token, 
        rn->tail->key->token, rn->l_str, rn->count);
}

// Hash table is an array of pointers to KeyNode structs.
typedef KeyNode* KeyHashTable[TABLE_SIZE];
typedef RuleNode* RuleHashTable[TABLE_SIZE];

#endif