#include "memoised.h"
#include "key_hash_table.c"
#include "rule_hash_table.c"

#define EMPTY_TOKEN 0x88

KeyHashTable key_strs;
RuleHashTable rule_strs;
KeyNode EMPTY_KEY = {.count = 0};

KeyNode* key_get_def(Token key, Grammar grammar, size_t l_str);
RuleNode* rules_get_def(Rule rule, Grammar grammar, size_t l_str);

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

KeyNode* key_get_def(Token key, Grammar grammar, size_t l_str)
{
    KeyNode* memoised_result = get_key(&key_strs, key, l_str);
    if (memoised_result != NULL)
        return memoised_result;

    size_t nt_index;
    if ((nt_index = is_non_terminal(key)) != -1)
    {
        NonTerminal nt = grammar.non_terminals[nt_index]; 

        RuleNode* s = NULL; 
        int count = 0;
        for (size_t i = 0; i < nt.num_rules; i++) {
            RuleNode* s_ = rules_get_def(nt.rules[i], grammar, l_str);

            int rule_count = 0;
            for (RuleNode* r = s_; r != NULL; r = r->next) 
            {
                rule_count += r->count;
            }

            count += rule_count; 

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
        }
        KeyNode* kn = malloc(sizeof(KeyNode));
        kn->token = key;
        kn->l_str = l_str;
        kn->count = count;
        kn->rules = s;

        insert_key(&key_strs, key, l_str, kn);
        return kn;
    }
    else
    {
        // `key` is a terminal symbol.
        if (l_str == key)
        {
            KeyNode* kn = malloc(sizeof(KeyNode));
            kn->token = key;
            kn->l_str = l_str;
            kn->count = 1;
            kn->rules = NULL;

            insert_key(&key_strs, key, l_str, kn);
            return kn;
        }
        else
        {
            insert_key(&key_strs, key, l_str, &EMPTY_KEY);     
            return &EMPTY_KEY;
        }
    }
}

RuleNode* rules_get_def(Rule rule, Grammar grammar, size_t l_str)
{
    if (rule.num_tokens == 0) return NULL;
    
    RuleNode* memoised_result = get_rule(&rule_strs, rule, l_str);
    if (memoised_result != NULL)
        return memoised_result;

    // The head is the first non-empty token.
    Token head = EMPTY_TOKEN;
    size_t head_index;
    for (head_index = 0; head_index < rule.num_tokens; head_index++)
    {
        if (rule.tokens[head_index] != EMPTY_TOKEN)
        {
            head = rule.tokens[head_index];
            break;
        }
    }
    if (head == EMPTY_TOKEN) return NULL;

    // If the head is the last token in the array, then there is no tail.
    if (head_index == rule.num_tokens - 1)
    {
        KeyNode* s_ = key_get_def(head, grammar, l_str);
        if (s_->count == 0) return NULL;

        RuleNode* rn = malloc(sizeof(RuleNode));
        rn->key = s_;
        rn->tail = NULL;
        rn->l_str = l_str;
        rn->count = s_->count;

        return rn;
    }

    Rule rule_copy = rule;
    rule_copy.tokens[head_index] = EMPTY_TOKEN;

    RuleNode* sum_rule = NULL;
    int count = 0;
    for (size_t partition = 1; partition <= l_str; partition++)
    {
        size_t h_len = partition;
        size_t t_len = l_str - partition;

        KeyNode* s_in_h = key_get_def(head, grammar, h_len);
        if (s_in_h->count == 0) continue;

        RuleNode* s_in_t = rules_get_def(rule_copy, grammar, t_len);
        if (s_in_t == NULL) continue;

        int count_ = 0;

        // Iterate through the linked list of RuleNode
        for (RuleNode* r = s_in_t; r != NULL; r = r->next)
        {
            count_ += s_in_h->count * r->count;
        }

        if (count_ == 0) continue;

        count += count_;

        // Create a new RuleNode for the current partition
        RuleNode* rn = malloc(sizeof(RuleNode));
        rn->key = s_in_h;
        rn->tail = s_in_t;
        rn->l_str = partition;
        rn->count = count_;

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
    insert_rule(&rule_strs, rule, l_str, sum_rule);

    return sum_rule;
}

void free_rule_node(RuleNode* rn) 
{
    RuleNode* current = rn;
    while (current != NULL) {
        RuleNode* next = current->next;
        free(current);
        current = next;
    }
    free(rn);
}

void free_key_node(KeyNode* kn) 
{
    KeyNode* current = kn;
    while (current != NULL) {
        KeyNode* next = current->next;
        free(current);
        current = next;
    }
    free(kn);
}

int main()
{
    init_key_hash_table(&key_strs);
    init_rule_hash_table(&rule_strs);

    KeyNode* key_node = key_get_def(0x1, GRAMMAR, 3);
    // printf("len: %d\n", key_node->count);

    return 0;
}