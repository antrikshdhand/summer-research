#include "memoised.h"
#include "key_hash_table.c"
#include "rule_hash_table.c"
#include "grammar_hash_table.c"

KeyHashTable key_strs;
RuleHashTable rule_strs;
GrammarHashTable grammar_hash;

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

int key_get_count(KeyNode* kn)
{
    if (kn->count == 1)
        return 1;

    int s_len = 0;
    for (RuleNode* ptr = kn->rules; ptr != NULL; ptr = ptr->next)
    {
        int s = rule_get_count(ptr);
        s_len += s;
    }
    return s_len;
}

int rule_get_count(RuleNode* rn)
{
    int s_len = 0;
    int s_k = key_get_count(rn->key);
    
    if (rn->tail == NULL)
        return s_k;

    for (RuleNode* ptr = rn->tail; ptr != NULL; ptr = ptr->next)
    {
        int s_t = rule_get_count(ptr);
        s_len = s_k * s_t;
    }

    return s_len;
}

// Compares two individual DTAs for equality.
int dtas_equal(DynamicTokenArray* dta1, DynamicTokenArray* dta2)
{
    if (dta1->length != dta2->length)
        return 0;
    
    for (size_t i = 0; i < dta1->length; i++)
    {
        if (dta1->list[i] != dta2->list[i])
        {
            return 0;
        }
    }

    return 1;
}

// Compares two DTA lists for equality.
int dta_lists_equal(DynamicTokenArray* dta_list1, DynamicTokenArray* dta_list2)
{
    int is_duplicate = 0;
    DynamicTokenArray* outer_ptr = dta_list1;
    DynamicTokenArray* inner_ptr = dta_list2;
    while (outer_ptr != NULL)
    {
        inner_ptr = dta_list2;
        while (inner_ptr != NULL)
        {
            if (dtas_equal(outer_ptr, inner_ptr))
            {
                is_duplicate = 1;
                break;
            }
            inner_ptr = inner_ptr->next_dta;
        }
        outer_ptr = outer_ptr->next_dta;
    }

    return is_duplicate;
}

// Append arr2 to the end of arr1.
void append_token_arrs(DynamicTokenArray** arr1, DynamicTokenArray* arr2) {
    // Handle empty list case:
    if (*arr1 == NULL) {
        *arr1 = arr2;
        return;
    }

    // Find the last node of the list:
    DynamicTokenArray* last = *arr1;
    while (last->next_dta != NULL) {
        last = last->next_dta;
    }

    // Append arr2 to the end of the list:
    arr2->next_dta = NULL; 
    last->next_dta = arr2;
}

// Concatenate the lists inside two token arrays into a third DTA.
DynamicTokenArray* concat_token_arrs(DynamicTokenArray* arr1, 
                                    DynamicTokenArray* arr2) 
{
    DynamicTokenArray* result = malloc(sizeof(DynamicTokenArray));
    result->length = arr1->length + arr2->length;
    result->list = malloc(result->length * sizeof(Token));
    result->next_dta = NULL;

    // Copy tokens from arr1
    for (size_t i = 0; i < arr1->length; i++) 
    {
        result->list[i] = arr1->list[i];
    }

    // Copy tokens from arr2
    for (size_t i = 0; i < arr2->length; i++) 
    {
        result->list[arr1->length + i] = arr2->list[i];
    }

    return result;
}

DynamicTokenArray* concat_dta_lists(DynamicTokenArray* list1, DynamicTokenArray* list2) {
    DynamicTokenArray* result = NULL;

    for (DynamicTokenArray* ptr1 = list1; ptr1 != NULL; ptr1 = ptr1->next_dta) 
    {
        for (DynamicTokenArray* ptr2 = list2; ptr2 != NULL; ptr2 = ptr2->next_dta)
        {
            DynamicTokenArray* concatenated = concat_token_arrs(ptr1, ptr2);
            append_token_arrs(&result, concatenated);
        }
    }

    return result;
}

void free_token_array(DynamicTokenArray* arr) {
    free(arr->list);
    free(arr);
}

void print_dta(DynamicTokenArray* dta, int stringify)
{
    if (dta == NULL)
    {
        printf("NULL\n");
        return;
    }

    for (size_t i = 0; i < dta->length; i++)
    {
        if (stringify)
        {
            printf("%s ", get_grammar(&grammar_hash, dta->list[i])->str);
        }
        else
        {
            printf("0x%x ", dta->list[i]);
        }
    }
    printf("\n");
}

void print_list_of_dtas(DynamicTokenArray* head, int stringify)
{
    if (head == NULL)
    {
        printf("NULL\n");
        return;
    }

    int i = 0;
    while (head != NULL)
    {
        printf("DTA linked-list node %d (len: %lu):\n", i, head->length); 
        print_dta(head, stringify);
        i++;
        head = head->next_dta;
    }
    printf("\n");
}

DynamicTokenArray* key_extract_strings(KeyNode* kn)
{
    if (kn->rules == NULL)
    {
        DynamicTokenArray* dta = malloc(sizeof(DynamicTokenArray));
        dta->length = 1;
        dta->list = malloc(dta->length * sizeof(Token));
        dta->list[0] = kn->token;
        dta->next_dta = NULL;

        return dta;
    }

    // A list of DTAs where a DTA represents one possible string.
    DynamicTokenArray* valid_strings = NULL;
    for (RuleNode* ptr = kn->rules; ptr != NULL; ptr = ptr->next)
    {
        DynamicTokenArray* s = rule_extract_strings(ptr);
        if (s == NULL)
            continue;

        append_token_arrs(&valid_strings, s); 
    } 

    return valid_strings;

}

DynamicTokenArray* rule_extract_strings(RuleNode* rn)
{
    DynamicTokenArray* s_h = key_extract_strings(rn->key);
    
    if (rn->tail == NULL)
        return s_h;

    DynamicTokenArray* valid_strings = NULL;
    for (RuleNode* ptr = rn->tail; ptr != NULL; ptr = ptr->next)
    {
        DynamicTokenArray* s_t = rule_extract_strings(ptr);
        
        DynamicTokenArray* s1 = s_h;
        while (s1 != NULL)
        {
            DynamicTokenArray* s2 = s_t;
            while (s2 != NULL)
            {
                // Concatenate s1 and s2 and append to valid_strings.
                DynamicTokenArray* tmp = concat_dta_lists(s1, s2);

                // Check for duplicates before appending.
                int is_duplicate = dta_lists_equal(valid_strings, tmp);
                if (!is_duplicate) {
                    append_token_arrs(&valid_strings, tmp);
                }

                s2 = s2->next_dta;
            }
            s1 = s1->next_dta;
        }
    } 

    return valid_strings;
}

int main()
{
    init_key_hash_table(&key_strs);
    init_rule_hash_table(&rule_strs);
    init_grammar_hash_table(&grammar_hash);

    /* MAIN CODE HERE */

    KeyNode* key_node = key_get_def(0x80, &GRAMMAR, 16);

    // int count = key_extract_strings(key_node);
    // printf("len: %d\n", count);

    // DynamicTokenArray* strings = key_extract_strings(key_node);
    // print_list_of_dtas(strings, 1);
    
    /* ***************  */

    breakdown_key_hash_table(&key_strs);
    breakdown_rule_hash_table(&rule_strs);
    breakdown_grammar_hash_table(&grammar_hash);

    return 0;
}