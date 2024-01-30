#include "hash_table.h"

#define EMPTY_TOKEN 0x88

int key_get_num_strings(Token token, Grammar* grammar, size_t l_str);
int rule_get_num_strings(Rule rule, Grammar* grammar, size_t l_str);

HashTable table;

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

/**
 * @brief Counts the number of strings of a given size `l_str` which can be
 * generated by a `token` of a `grammar`. 
 *
 * @note Either:
 * @note a) `token` is a terminal. As a terminal will only produce itself,
 *          we just need to check if len(token) == l_str.
 * @note b) `token` is a non-terminal. Then the total number of strings of
 *          a given size which can be produced by the token is the total number
 *          of strings of a given size which can be produced by each of its
 *          decompositions.
 * @param token The Token from which to begin generation of strings from.
 * @param grammar The Grammar struct representing a BNF grammar.
 * @param l_str The length of the string to generate.
 * @return int The number of strings of length `l_str` which can be generated from `token`.
 */
int key_get_num_strings(Token token, Grammar* grammar, size_t l_str)
{
    size_t nt_index;
    if ((nt_index = is_non_terminal(token)) != -1)
    {
        int num_strings = 0;
        for (size_t i = 0; i < grammar->non_terminals[nt_index].num_rules; i++)
        {
            num_strings += rule_get_num_strings(
                grammar->non_terminals[nt_index].rules[i],
                grammar,
                l_str
            );
        }
        return num_strings;
    }

    // If it is a terminal symbol, we just have to check if the length matches.
    if (get(&table, token)->strlen == l_str) return 1;
    return 0;
}

/**
 * @brief Counts the number of strings of size `l_str` which can be generated
 * from a `rule` in a `grammar`.
 * 
 * @note We treat each rule as having a head followed by a tail.
 * @note e.g. [<digits>, <operator>, <digits>] would decompose to:
 * @note head = <digits>
 * @note tail = [<operator>, <digits>]
 * @note But in this implementation, rather than creating a new `tail` array, 
 * we copy the existing `rule.tokens` array and set the head to `EMPTY_TOKEN`.
 * Therefore, all tokens which are not `EMPTY_TOKEN` are the tail.
 * We then pass rule_copy into the recursive call.
 * 
 * @param rule The Rule struct from which to generate strings from.
 * @param grammar The Grammar struct representing a BNF grammar.
 * @param l_str The length of the string to generate.
 * @return int The number of strings of length `l_str` which can be generated from `rule`.
 */
int rule_get_num_strings(Rule rule, Grammar* grammar, size_t l_str)
{
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
    if (head == EMPTY_TOKEN) return 0;

    // If the head is the last token in the array, then there is no tail.
    if (head_index == rule.num_tokens - 1)
    {
        return key_get_num_strings(head, grammar, l_str);
    }

    Rule rule_copy = rule;
    rule_copy.tokens[head_index] = EMPTY_TOKEN;

    int sum_rule = 0;
    for (int partition = 1; partition < l_str + 1; partition++)
    {
        int h_len = partition;
        int t_len = l_str - partition;

        int str_in_head = key_get_num_strings(head, grammar, h_len);
        if (str_in_head == 0) continue;

        int str_in_tail = rule_get_num_strings(rule_copy, grammar, t_len);
        sum_rule += str_in_head * str_in_tail;
    }

    return sum_rule;
}

int main()
{
    init_hash_table(&table);

    TokenStr ts1 = {0x80, "<start>"};
    TokenStr ts2 = {0x81, "<sentence>"};
    TokenStr ts3 = {0x82, "<noun_phrase>"};
    TokenStr ts4 = {0x83, "<verb>"};
    TokenStr ts5 = {0x84, "<article>"};
    TokenStr ts6 = {0x85, "<noun>"};
    TokenStr ts7 = {0x00, "horse", 5};
    TokenStr ts8 = {0x01, "dog", 3};
    TokenStr ts9 = {0x02, "hamster", 7};
    TokenStr ts10 = {0x03, "a", 1};
    TokenStr ts11 = {0x04, "the", 3};
    TokenStr ts12 = {0x05, "stands", 6};
    TokenStr ts13 = {0x06, "walks", 5};
    TokenStr ts14 = {0x07, "jumps", 5};

    insert(&table, &ts1);
    insert(&table, &ts2);
    insert(&table, &ts3);
    insert(&table, &ts4);
    insert(&table, &ts5);
    insert(&table, &ts6);
    insert(&table, &ts7);
    insert(&table, &ts8);
    insert(&table, &ts9);
    insert(&table, &ts10);
    insert(&table, &ts11);
    insert(&table, &ts12);
    insert(&table, &ts13);
    insert(&table, &ts14);

    /**
     * Number of strings that can be generated from <noun_phrase>:
     * 
     * | Length | Count |
     * |--------|-------|
     * | 4      | 1     |
     * | 6      | 2     |
     * | 8      | 2     |
     * | 10     | 1     |
     * 
     */

    printf("count: %i\n", key_get_num_strings(0x82, &GRAMMAR, 6));
    // Output: 2

    /**
     * Number of strings that can be generated from <start>:
     * 
     * | Length | Count |
     * |--------|-------|
     * | 9      | 2     |
     * | 10     | 1     |
     * | 11     | 4     |
     * | 12     | 2     |
     * | 13     | 4     |
     * | 14     | 2     |
     * | 15     | 2     |
     * | 16     | 1     |
     * 
     */

    printf("count: %i\n", key_get_num_strings(0x80, &GRAMMAR, 11));
    // Output: 4

    return 0;
}
