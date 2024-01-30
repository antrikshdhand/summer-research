#include "memoised.h"

int hash_rule(Rule* rule, size_t l_str)
{
    uint32_t hash = 5382;

    for (size_t i = 0; i < rule->num_tokens; i++)
    {
        hash = ((hash << 5) + hash) ^ rule->tokens[i];
    }
    hash = ((hash << 5) + hash) ^ (uint32_t)l_str;

    return hash % RULE_TABLE_SIZE;
}

void init_rule_hash_table(RuleHashTable* table)
{
    // Set entire table to be empty.
    for (size_t i = 0; i < RULE_TABLE_SIZE; i++)
    {
        (*table)[i] = NULL;
    } 
}

void print_rule_hash_table(RuleHashTable* table)
{
    printf("------------ RULE HASH TABLE --------------\n");
    for (size_t i = 0; i < RULE_TABLE_SIZE; i++)
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
                printf("(0x%x, l: %lu, c: %d, tail: [", tmp->key->token, tmp->l_str, tmp->count);
                
                // Print the tail linked list
                RuleNode* tailNode = tmp->tail;
                while (tailNode != NULL) {
                    printf("(0x%x, l: %lu, c: %d) -> ", tailNode->key->token, tailNode->l_str, tailNode->count);
                    tailNode = tailNode->next;
                }
                
                printf("]) -> ");
                
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("--------------------------------------------\n");
}

void insert_rule(RuleHashTable* table, Rule* rule, size_t l_str, RuleNode* rn)
{
    if (rn == NULL) return;

    int index = hash_rule(rule, l_str);

    // Replace the current start of the linked-list with rn.
    RuleNode* last = rn;
    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = (*table)[index];
    (*table)[index] = rn;
}

int rule_node_equals(RuleNode* tmp, Rule* rule, size_t l_str) 
{
    if (tmp->key->token != rule->tokens[0])
        return 0;

    if (tmp->l_str != l_str)
        return 0;

    if (tmp->tail->key->token != rule->tokens[1])
        return 0;

    return 1;
}

RuleNode* get_rule(RuleHashTable* table, Rule* rule, size_t l_str)
{
    int index = hash_rule(rule, l_str);
    RuleNode* tmp = (*table)[index];
    while (tmp != NULL) 
    {
        if (rule_node_equals(tmp, rule, l_str)) 
        {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

void free_rule_node(RuleNode* rn) 
{
    RuleNode* current = rn;
    while (current != NULL) {
        RuleNode* next = current->next;
        free(current);
        current = next;
    }
}

void breakdown_rule_hash_table(RuleHashTable* table)
{
    for (size_t i = 0; i < RULE_TABLE_SIZE; i++)
    {
        free_rule_node((*table)[i]);
    }
}