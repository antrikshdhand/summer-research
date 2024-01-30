#ifndef MEMOISED_H
#define MEMOISED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../fuzzer/src/c/grammar.h"

// Use the print_%_hash_table functions to adjust empirically.
#define GRAMMAR_TABLE_SIZE 20
#define KEY_TABLE_SIZE 60 
#define RULE_TABLE_SIZE 30 

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
typedef struct RuleHashTableVal RuleHashTableVal;

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

// Our RuleHashTable values are linked-lists filled with RuleNodes
struct RuleHashTableVal
{
    RuleNode* list;
    size_t l_str;
    struct RuleHashTableVal* next;
};

typedef struct TokenStr
{
    Token key;
    char* str;
    size_t strlen;
    struct TokenStr* next; // External chaining.
} TokenStr;

// Hash table is an array of pointers to structs.
typedef KeyNode* KeyHashTable[KEY_TABLE_SIZE];
typedef RuleHashTableVal* RuleHashTable[RULE_TABLE_SIZE];
typedef TokenStr* GrammarHashTable[GRAMMAR_TABLE_SIZE];

#endif