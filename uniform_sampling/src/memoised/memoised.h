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
typedef struct 
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
    Token token; 
    size_t l_str;
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

// Our RuleHashTable values are linked-lists filled with RuleNodes.
struct RuleHashTableVal
{
    RuleNode* list;
    size_t l_str;
    struct RuleHashTableVal* next;
};

typedef struct GrammarHashTableVal 
{
    Token key;
    char* str;
    size_t strlen;
    struct GrammarHashTableVal* next; // External chaining.
} GrammarHashTableVal;

// A TokenArray represents a "string"
typedef struct DynamicTokenArray
{
    Token* list;
    size_t length;
    struct DynamicTokenArray* next_dta;
} DynamicTokenArray;

// Hash table is an array of pointers to structs.
typedef KeyNode* KeyHashTable[KEY_TABLE_SIZE];
typedef RuleHashTableVal* RuleHashTable[RULE_TABLE_SIZE];
typedef GrammarHashTableVal* GrammarHashTable[GRAMMAR_TABLE_SIZE];

// Function definitions
KeyNode* key_get_def(Token key, Grammar* grammar, size_t l_str);
RuleNode* rules_get_def(Rule* rule, Grammar* grammar, size_t l_str);
int key_get_count(KeyNode* kn);
int rule_get_count(RuleNode* rn);
DynamicTokenArray* key_extract_strings(KeyNode* kn);
DynamicTokenArray* rule_extract_strings(RuleNode* rn);


#endif