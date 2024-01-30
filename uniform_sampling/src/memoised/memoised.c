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

KeyNode* create_key_node(Token key, size_t l_str, int count, RuleNode* rules)
{
    KeyNode* kn = malloc(sizeof(KeyNode));
    kn->token = key;
    kn->l_str = l_str;
    kn->count = count;
    kn->rules = rules;
    kn->next = NULL;

    return kn;
}

RuleNode* create_rule_node(KeyNode* key, RuleNode* tail, 
                            size_t l_str, int count)
{
    RuleNode* rn = malloc(sizeof(RuleNode));
    rn->key = key;
    rn->tail = tail;
    rn->l_str = l_str;
    rn->count = count;
    rn->next = NULL;

    return rn;
}

KeyNode* key_get_def(Token key, Grammar* grammar, size_t l_str)
{
    if (get_key(&key_strs, key, l_str) != NULL)
        return get_key(&key_strs, key, l_str);
    
    size_t nt_index;
    if ((nt_index = is_non_terminal(key)) != -1)
    {
        RuleNode* s = NULL; 
        int count = 0;
        for (size_t i = 0; i < grammar->non_terminals[nt_index].num_rules; i++) {
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

            // Append s_ to s.
            if (s == NULL)
            {
                s = s_;
            }
            else
            { 
                RuleNode* last = s;
                while (last->next != NULL)
                {
                    last = last->next;
                }
                last->next = s_;
            }
        }

        KeyNode* kn = create_key_node(key, l_str, count, s);
        insert_key(&key_strs, key, l_str, kn);
        return kn;
    }
    else
    {
        // `key` is a terminal symbol.
        if (l_str == get_grammar(&grammar_hash, key)->strlen)
        {
            KeyNode* kn = create_key_node(key, l_str, 1, NULL);
            insert_key(&key_strs, key, l_str, kn);
            return kn;
        }
        else
        {
            KeyNode* empty_key = create_key_node(EMPTY_TOKEN, 0, -1, NULL);
            return empty_key;
        }
    }
}

RuleNode* rules_get_def(Rule* rule, Grammar* grammar, size_t l_str)
{
    if (rule->num_tokens == 0) 
        return NULL;
    
    RuleNode* memoised_result;
    if ((memoised_result = get_rule(&rule_strs, rule, l_str)) != NULL)
        return memoised_result;

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

    // If the head is the last token in the array, then there is no tail.
    if (head_index == rule->num_tokens - 1)
    {
        KeyNode* s_ = key_get_def(head, grammar, l_str);
        if (s_->count == -1) 
            return NULL;

        RuleNode* rn = create_rule_node(s_, NULL, l_str, s_->count);
        return rn;
    }

    // Create a new rule with head_index set to EMPTY_TOKEN.
    Rule* rule_copy = malloc(sizeof(Rule));
    rule_copy->num_tokens = rule->num_tokens;
    for (size_t i = 0; i < rule->num_tokens; i++)
    {
        rule_copy->tokens[i] = rule->tokens[i];
    }
    rule_copy->tokens[head_index] = EMPTY_TOKEN;

    RuleNode* sum_rule = NULL; // List of RuleNodes
    for (size_t partition = 1; partition <= l_str; partition++)
    {
        size_t h_len = partition;
        size_t t_len = l_str - partition;

        KeyNode* s_in_h = key_get_def(head, grammar, h_len);
        if (s_in_h->count == -1) 
            continue;

        RuleNode* s_in_t = rules_get_def(rule_copy, grammar, t_len);
        if (s_in_t == NULL) 
            continue;

        int count = 0;
        for (RuleNode* r = s_in_t; r != NULL; r = r->next)
        {
            count += s_in_h->count * r->count;
        }

        if (count == 0) 
            continue;

        // Create a new RuleNode for the current partition.
        RuleNode* rn = create_rule_node(s_in_h, s_in_t, partition, count);

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

    // Memoise.
    if (sum_rule != NULL)
        insert_rule(&rule_strs, rule_copy, l_str, sum_rule);

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
        KeyNode* key_node = key_get_def(0x81, &GRAMMAR, l_str);
        printf("l_str = %lu, count = %d\n", l_str, key_node->count);
    }
    
    /* ***************  */

    breakdown_key_hash_table(&key_strs);
    breakdown_rule_hash_table(&rule_strs);
    breakdown_grammar_hash_table(&grammar_hash);

    return 0;
}