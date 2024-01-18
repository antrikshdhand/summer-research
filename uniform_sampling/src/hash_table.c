#include <stdio.h>
#include <stdint.h>

#define MAX_TOKEN_LEN 50
#define TABLE_SIZE 100

typedef struct TokenStr
{
    char str[MAX_TOKEN_LEN];
    int strlen;
} TokenStr;

// Our hash table implementation is an array of pointers to TokenStr structs.
typedef TokenStr* HashTable[TABLE_SIZE];

unsigned int hash(uint8_t key)
{
    // Transform the 8-bit key into an array index.
    
    return 0;
}

void init_hash_table(HashTable* table)
{
    // Set entire table to be empty
    for (size_t i = 0; i < TABLE_SIZE; i++)
    {
        (*table)[i] = NULL;
    } 
}

void print_hash_table(HashTable* table)
{
    printf("-------------------------------\n");
    for (size_t i = 0; i < TABLE_SIZE; i++)
    {
        if ((*table)[i] == NULL)
        {
            printf("\t%zu\t---\n", i);
        }
        else
        {
            printf("\t%zu\t\n", i);
            // Person* tmp = hash_table[i];
            // while (tmp != NULL) {
            //     printf("%s -> ", tmp->name);
            //     tmp = tmp->next;
            // }
            // printf("\n");
        }
    }
    printf("-------------------------------\n");
}

