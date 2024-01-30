#include "hash_table.h"

int hash(Token key)
{
    // Jenkins one-at-a-time hash
    uint32_t hash = 0;

    for (size_t i = 0; i < 8; ++i) {
        hash += key & 1;
        hash += (hash << 10);
        hash ^= (hash >> 6);
        key >>= 1;
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % KEY_TABLE_SIZE;
}

void init_hash_table(HashTable* table)
{
    // Set entire table to be empty.
    for (size_t i = 0; i < KEY_TABLE_SIZE; i++)
    {
        (*table)[i] = NULL;
    } 
}

void print_hash_table(HashTable* table)
{
    printf("-------------------------------\n");
    for (size_t i = 0; i < KEY_TABLE_SIZE; i++)
    {
        if ((*table)[i] == NULL)
        {
            printf("\t%zu\t---\n", i);
        }
        else
        {
            printf("\t%zu\t", i);
            TokenStr* tmp = (*table)[i];
            while (tmp != NULL) {
                printf("%s -> ", tmp->str);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("-------------------------------\n");
}

int insert(HashTable* table, TokenStr* ts)
{
    if (ts == NULL) return -1;
    int index = hash(ts->key);

    // Replace the current start of the linked-list with p.
    ts->next = (*table)[index];
    (*table)[index] = ts;

    return 0;
}

TokenStr* get(HashTable* table, Token key)
{
    int index = hash(key);
    TokenStr* tmp = (*table)[index];
    while (tmp != NULL && tmp->key != key)
    {
        tmp = tmp->next;
    }
    return tmp;
}

TokenStr* delete(HashTable* table, Token key)
{
    int index = hash(key);
    
    TokenStr* tmp = (*table)[index];
    TokenStr* prev = NULL;
    while (tmp != NULL && tmp->key != key)
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

// int main()
// {
//     HashTable table;

//     TokenStr ts1 = {0x80, "<start>"};
//     TokenStr ts2 = {0x81, "<sentence>"};
//     TokenStr ts3 = {0x82, "<noun_phrase>"};
//     TokenStr ts4 = {0x83, "<verb>"};
//     TokenStr ts5 = {0x84, "<article>"};
//     TokenStr ts6 = {0x85, "<noun>"};
//     TokenStr ts7 = {0x00, "horse", 5};
//     TokenStr ts8 = {0x01, "dog", 3};
//     TokenStr ts9 = {0x02, "hamster", 7};
//     TokenStr ts10 = {0x03, "a", 1};
//     TokenStr ts11 = {0x04, "the", 3};
//     TokenStr ts12 = {0x05, "stands", 6};
//     TokenStr ts13 = {0x06, "walks", 6};
//     TokenStr ts14 = {0x07, "jumps", 6};

//     insert(&table, &ts1);
//     insert(&table, &ts2);
//     insert(&table, &ts3);
//     insert(&table, &ts4);
//     insert(&table, &ts5);
//     insert(&table, &ts6);
//     insert(&table, &ts7);
//     insert(&table, &ts8);
//     insert(&table, &ts9);
//     insert(&table, &ts10);
//     insert(&table, &ts11);
//     insert(&table, &ts12);
//     insert(&table, &ts13);
//     insert(&table, &ts14);

//     printf("getting 0x80: %s\n", get(&table, 0x80)->str);
//     printf("getting 0x81: %s\n", get(&table, 0x81)->str);
//     printf("getting 0x82: %s\n", get(&table, 0x82)->str);
//     printf("getting 0x83: %s\n", get(&table, 0x83)->str);
//     printf("getting 0x84: %s\n", get(&table, 0x84)->str);
//     printf("getting 0x85: %s\n", get(&table, 0x85)->str);
//     printf("getting 0x00: %s\n", get(&table, 0x00)->str);
//     printf("getting 0x01: %s\n", get(&table, 0x01)->str);
//     printf("getting 0x02: %s\n", get(&table, 0x02)->str);
//     printf("getting 0x03: %s\n", get(&table, 0x03)->str);
//     printf("getting 0x04: %s\n", get(&table, 0x04)->str);
//     printf("getting 0x05: %s\n", get(&table, 0x05)->str);
//     printf("getting 0x06: %s\n", get(&table, 0x06)->str);
//     printf("getting 0x07: %s\n", get(&table, 0x07)->str);
// }