#include "memoised.h"

int hash_key(Token key, size_t l_str)
{
    uint32_t hash = 17;

    // Combine the key and l_str into the hash
    hash = hash * 31 + key;
    hash = hash * 31 + (uint32_t)l_str;

    return hash % TABLE_SIZE;
}

void init_key_hash_table(KeyHashTable* table)
{
    // Set entire table to be empty.
    for (size_t i = 0; i < TABLE_SIZE; i++)
    {
        (*table)[i] = NULL;
    } 
}

void print_key_hash_table(KeyHashTable* table)
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
            printf("\t%zu\t", i);
            KeyNode* tmp = (*table)[i];
            while (tmp != NULL) {
                printf("0x%x <len: %lu> -> ", tmp->token, tmp->l_str);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("-------------------------------\n");
}

void insert_key(KeyHashTable* table, Token key, size_t l_str, KeyNode* kn)
{
    int index = hash_key(key, l_str);

    // Replace the current start of the linked-list with kn.
    kn->next = (*table)[index];
    (*table)[index] = kn;
}

KeyNode* get_key(KeyHashTable* table, Token key, size_t l_str)
{
    int index = hash_key(key, l_str);
    KeyNode* tmp = (*table)[index];
    while (tmp != NULL && (tmp->token != key && tmp->l_str != l_str))
    {
        tmp = tmp->next;
    }
    return tmp;
}

KeyNode* delete_key(KeyHashTable* table, Token key, size_t l_str)
{
    int index = hash_key(key, l_str);
    
    KeyNode* tmp = (*table)[index];
    KeyNode* prev = NULL;
    while (tmp != NULL && tmp->token != key)
    {
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL) return NULL; // No match found.

    if (prev == NULL)
    {
        // The node to delete was the head of the linked-list.
        (*table)[index] = tmp->next;
    }
    else
    {
        prev->next = tmp->next;
    }

    return tmp;
}