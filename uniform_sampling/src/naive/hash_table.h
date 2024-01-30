#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include "../../fuzzer/src/c/grammar.h"

#define MAX_TOKEN_LEN 50
#define TABLE_SIZE 25 

typedef struct TokenStr
{
    uint8_t key;
    char str[MAX_TOKEN_LEN];
    int strlen;
    struct TokenStr* next; // External chaining.
} TokenStr;

// Our hash table implementation is an array of pointers to TokenStr structs.
typedef TokenStr* HashTable[KEY_TABLE_SIZE];

int hash(Token key);
void init_hash_table(HashTable* table);
void print_hash_table(HashTable* table);
int insert(HashTable* table, TokenStr* ts);
TokenStr* get(HashTable* table, Token key);
TokenStr* delete(HashTable* table, Token key);

#endif 