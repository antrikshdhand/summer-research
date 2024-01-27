#include "memoised.h"

int hash_rule(Rule rule, size_t l_str)
{
    uint32_t hash = 17;

    // Combine the rule's keys and l_str into the hash
    for (size_t i = 0; i < rule.num_tokens; i++)
    {
        hash = hash * 31 + rule.tokens[i];
    }
    hash = hash * 31 + (uint32_t)l_str;

    return hash % TABLE_SIZE;
}

void init_rule_hash_table(RuleHashTable* table)
{
    // Set entire table to be empty.
    for (size_t i = 0; i < TABLE_SIZE; i++)
    {
        (*table)[i] = NULL;
    } 
}

void print_rule_hash_table(RuleHashTable* table)
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
            RuleNode* tmp = (*table)[i];
            while (tmp != NULL) {
                printf("0x%x <len: %lu> -> ", tmp->key->token, tmp->l_str);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("-------------------------------\n");
}

void insert_rule(RuleHashTable* table, Rule rule, size_t l_str, RuleNode* rn)
{
    int index = hash_rule(rule, l_str);

    // Replace the current start of the linked-list with kn.
    rn->next = (*table)[index];
    (*table)[index] = rn;
}

RuleNode* get_rule(RuleHashTable* table, Rule rule, size_t l_str)
{
    int index = hash_rule(rule, l_str);
    RuleNode* tmp = (*table)[index];
    while (tmp != NULL && (tmp->key->token != rule.tokens[0] && tmp->l_str != l_str))
    {
        tmp = tmp->next;
    }
    return tmp;
}

RuleNode* delete_rule(RuleHashTable* table, Rule rule, size_t l_str)
{
    int index = hash_rule(rule, l_str);
    
    RuleNode* tmp = (*table)[index];
    RuleNode* prev = NULL;
    while (tmp != NULL && tmp->key->token != rule.tokens[0])
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