#include "memoised.h"
#include "key_hash_table.c"
#include "rule_hash_table.c"
#include "grammar_hash_table.c"

KeyHashTable key_strs;
RuleHashTable rule_strs;
GrammarHashTable grammar_hash;

KeyNode* key_get_def(Token key, Grammar* grammar, size_t l_str);
RuleNode* rules_get_def(Rule* rule, Grammar* grammar, size_t l_str);

Grammar GRAMMAR = {
	6,
	{
		{
			// <start>
			0x80,
			1,
			{
				{
					// <sentence>
					1,
					{0x81}
				}
			}
		},
		{
			// <sentence>
			0x81,
			1,
			{
				{
					// <noun_phrase>, <verb>
					2,
					{0x82, 0x83}
				}
			}
		},
		{
			// <noun_phrase>
			0x82,
			1,
			{
				{
					// <article>, <noun>
					2,
					{0x84, 0x85}
				}
			}
		},
		{
			// <verb>
			0x83,
			3,
			{
				{
					// stands
					1,
					{0x5}
				},
				{
					// walks
					1,
					{0x6}
				},
				{
					// jumps
					1,
					{0x7}
				}
			}
		},
		{
			// <article>
			0x84,
			2,
			{
				{
					// a
					1,
					{0x3}
				},
				{
					// the
					1,
					{0x4}
				}
			}
		},
		{
			// <noun>
			0x85,
			3,
			{
				{
					// horse
					1,
					{0x0}
				},
				{
					// dog
					1,
					{0x1}
				},
				{
					// hamster
					1,
					{0x2}
				}
			}
		}
	}
};

KeyNode* key_get_def(Token key, Grammar* grammar, size_t l_str)
{
    // printf("\nEntering key_get_def()\n");
    // printf("key: 0x%x ", key);

    if (get_key(&key_strs, key, l_str) != NULL)
    {
        // printf(" --> returning\n");
        return get_key(&key_strs, key, l_str);
    }
    
    size_t nt_index;
    if ((nt_index = is_non_terminal(key)) != -1)
    {
        // printf("(non-terminal)\n");
        RuleNode* s = NULL; 
        int count = 0;
        for (size_t i = 0; i < grammar->non_terminals[nt_index].num_rules; i++) {
            // printf("\nFOR-LOOP ITER: %lu (rule[0] = 0x%x)\n", i, grammar->non_terminals[nt_index].rules[i].tokens[0]);

            RuleNode* s_ = rules_get_def(
                &grammar->non_terminals[nt_index].rules[i], 
                grammar, 
                l_str
            );

            if (s_ == NULL) 
                continue;

            for (RuleNode* r = s_; r != NULL; r = r->next) 
            {
                count += r->count;
            }

            if (s == NULL)
            {
                s = s_;
            }
            else
            { 
                // Append s_ to the end of s
                RuleNode* last = s;
                while (last->next != NULL)
                {
                    last = last->next;
                }
                last->next = s_;
            }

            // Print s for debug
            // printf("Printing s\n");
            // for (RuleNode* r = s; r != NULL; r = r->next) 
            // {
            //     print_rule_node(r);
            // }
        }

        // printf("EXITED FOR-LOOP.\n");
        
        KeyNode* kn = malloc(sizeof(KeyNode));
        kn->token = key;
        kn->l_str = l_str;
        kn->count = count;
        kn->rules = s;
        kn->next = NULL;

        // printf("Printing kn:\n");
        // print_key_node(kn);

        insert_key(&key_strs, key, l_str, kn);
        return kn;
    }
    else
    {
        // `key` is a terminal symbol.
        // printf("(terminal)\n");
        // printf("l_str = %lu, len(key) = %lu\n", l_str, get_grammar(&grammar_hash, key)->strlen);

        if (l_str == get_grammar(&grammar_hash, key)->strlen)
        {
            // printf("Lengths match\n");
            KeyNode* kn = malloc(sizeof(KeyNode));
            kn->token = key;
            kn->l_str = l_str;
            kn->count = 1;
            kn->rules = NULL;
            kn->next = NULL;

            insert_key(&key_strs, key, l_str, kn);
            return kn;
        }
        else
        {
            // printf("Lengths do not match\n");
            KeyNode* empty_key = malloc(sizeof(KeyNode));
            empty_key->token = EMPTY_TOKEN;
            empty_key->l_str = 0;
            empty_key->count = -1;
            empty_key->rules = NULL;
            empty_key->next = NULL;
            return empty_key;
        }
    }
}

RuleNode* rules_get_def(Rule* rule, Grammar* grammar, size_t l_str)
{
    // printf("Entering rules_get_def(), l_str = %lu\n", l_str);
    if (rule->num_tokens == 0) 
    {
        // printf("rule.num_tokens == 0, returning NULL\n");
        return NULL;
    }
    
    // RuleNode* memoised_result;
    // if ((memoised_result = get_rule(&rule_strs, rule, l_str)) != NULL)
    // {
    //     printf("memoised entry found! returning...\n");
    //     return memoised_result;
    // }

    // The head is the first non-empty token.
    Token head = EMPTY_TOKEN;
    size_t head_index;
    for (head_index = 0; head_index < rule->num_tokens; head_index++)
    {
        if (rule->tokens[head_index] != EMPTY_TOKEN)
        {
            head = rule->tokens[head_index];
            break;
        }
    }
    if (head == EMPTY_TOKEN) return NULL;

    // printf("head: 0x%x ", head);

    // If the head is the last token in the array, then there is no tail.
    if (head_index == rule->num_tokens - 1)
    {
        // printf("(no tail)\n");
        KeyNode* s_ = key_get_def(head, grammar, l_str);
        if (s_->count == -1) 
        {
            // printf("count == -1, returning NULL\n");
            return NULL;
        }

        RuleNode* rn = malloc(sizeof(RuleNode));
        rn->key = s_;
        rn->tail = NULL;
        rn->l_str = l_str;
        rn->count = s_->count;
        rn->next = NULL;
        // printf("Returning from rules_get_def() with rn->key = 0x%x\n\n", rn->key->token);
        return rn;
    }

    // printf("(tail = [0x%x, ...])\n", rule.tokens[head_index + 1]);

    Rule* rule_copy = malloc(sizeof(Rule));
    rule_copy->num_tokens = rule->num_tokens;
    for (size_t i = 0; i < rule->num_tokens; i++)
    {
        rule_copy->tokens[i] = rule->tokens[i];
    }
    rule_copy->tokens[head_index] = EMPTY_TOKEN;

    // List of RuleNodes
    RuleNode* sum_rule = NULL;
    for (size_t partition = 1; partition < l_str; partition++)
    {
        // printf("partition: %lu\n", partition);
        size_t h_len = partition;
        size_t t_len = l_str - partition;

        KeyNode* s_in_h = key_get_def(head, grammar, h_len);
        if (s_in_h->count == -1) continue;

        RuleNode* s_in_t = rules_get_def(rule_copy, grammar, t_len);
        if (s_in_t == NULL) continue;

        int count = 0;
        for (RuleNode* r = s_in_t; r != NULL; r = r->next)
        {
            count += s_in_h->count * r->count;
        }

        if (count == 0) continue;

        // Create a new RuleNode for the current partition
        RuleNode* rn = malloc(sizeof(RuleNode));
        rn->key = s_in_h;
        rn->tail = s_in_t;
        rn->l_str = partition;
        rn->count = count;
        rn->next = NULL;

        // Append rn to the linked list
        if (sum_rule == NULL)
        {
            sum_rule = rn;
        }
        else
        {
            RuleNode* last = sum_rule;
            while (last->next != NULL)
            {
                last = last->next;
            }
            last->next = rn;
        }
    }

    // Memoize the result
    // insert_rule(&rule_strs, rule, l_str, sum_rule);

    return sum_rule;
}

int main()
{
    init_key_hash_table(&key_strs);
    init_rule_hash_table(&rule_strs);
    init_grammar_hash_table(&grammar_hash);

    /* MAIN CODE HERE */

    for (size_t l_str = 1; l_str <= 20; l_str++)
    {
        // printf("******** l_str = %lu ********\n", l_str);
        KeyNode* key_node = key_get_def(0x82, &GRAMMAR, l_str);
        // print_key_hash_table(&key_strs);
        // print_rule_hash_table(&rule_strs);
        // printf("count = %d\n", key_node->count);
        printf("l_str = %lu, count = %d\n", l_str, key_node->count);
    }
    
    /* ***************  */

    breakdown_key_hash_table(&key_strs);
    breakdown_rule_hash_table(&rule_strs);
    breakdown_grammar_hash_table(&grammar_hash);

    return 0;
}