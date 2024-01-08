# Taken from https://rahul.gopinath.org/post/2019/05/28/simplefuzzer-01/
# "The simplest grammar fuzzer in the world"

import random

sample_grammar = {
    '<start>': [['<json>']],
    '<json>': [['<element>']],
    '<element>': [['<ws>', '<value>', '<ws>']],
    '<value>': [
        ['<object>'], ['<array>'], ['<string>'], ['<number>'],
        ['true'], ['false'], ['null']],
    '<object>': [['{', '<ws>', '}'], ['{', '<members>', '}']],
    '<members>': [['<member>', '<symbol-2>']],
    '<member>': [['<ws>', '<string>', '<ws>', ':', '<element>']],
    '<array>': [['[', '<ws>', ']'], ['[', '<elements>', ']']],
    '<elements>': [['<element>', '<symbol-1-1>']],
    '<string>': [['"', '<characters>', '"']],
    '<characters>': [['<character-1>']],
    '<character>': [
        ['0'], ['1'], ['2'], ['3'], ['4'], ['5'], ['6'], ['7'], ['8'], ['9'],
        ['a'], ['b'], ['c'], ['d'], ['e'], ['f'], ['g'], ['h'], ['i'], ['j'],
        ['k'], ['l'], ['m'], ['n'], ['o'], ['p'], ['q'], ['r'], ['s'], ['t'],
        ['u'], ['v'], ['w'], ['x'], ['y'], ['z'], ['A'], ['B'], ['C'], ['D'],
        ['E'], ['F'], ['G'], ['H'], ['I'], ['J'], ['K'], ['L'], ['M'], ['N'],
        ['O'], ['P'], ['Q'], ['R'], ['S'], ['T'], ['U'], ['V'], ['W'], ['X'],
        ['Y'], ['Z'], ['!'], ['#'], ['$'], ['%'], ['&'], ["'"], ['('], [')'],
        ['*'], ['+'], [','], ['-'], ['.'], ['/'], [':'], [';'], ['<'], ['='],
        ['>'], ['?'], ['@'], ['['], [']'], ['^'], ['_'], ['`'], ['{'], ['|'],
        ['}'], ['~'], [' '], ['\\"'], ['\\\\'], ['\\/'], ['<escaped>']],
    '<number>': [['<int>', '<frac>', '<exp>']],
    '<int>': [
        ['<digit>'], ['<onenine>', '<digits>'],
        ['-', '<digits>'], ['-', '<onenine>', '<digits>']],
    '<digits>': [['<digit-1>']],
    '<digit>': [['0'], ['<onenine>']],
    '<onenine>': [['1'], ['2'], ['3'], ['4'], ['5'], ['6'], ['7'], 
                  ['8'], ['9']],
    '<frac>': [[], ['.', '<digits>']],
    '<exp>': [[], ['E', '<sign>', '<digits>'], ['e', '<sign>', '<digits>']],
    '<sign>': [[], ['+'], ['-']],
    '<ws>': [['<sp1>', '<ws>'], []],
    '<sp1>': [[' ']], ##[['\n'], ['\r'], ['\t'], ['\x08'], ['\x0c']],
    '<symbol>': [[',', '<members>']],
    '<symbol-1>': [[',', '<elements>']],
    '<symbol-2>': [[], ['<symbol>', '<symbol-2>']],
    '<symbol-1-1>': [[], ['<symbol-1>', '<symbol-1-1>']],
    '<character-1>': [[], ['<character>', '<character-1>']],
    '<digit-1>': [['<digit>'], ['<digit>', '<digit-1>']],
    '<escaped>': [['\\u', '<hex>', '<hex>', '<hex>', '<hex>']],
    '<hex>': [
        ['0'], ['1'], ['2'], ['3'], ['4'], ['5'], ['6'], ['7'], ['8'], ['9'],
        ['a'], ['b'], ['c'], ['d'], ['e'], ['f'], ['A'], ['B'], ['C'], ['D'], 
        ['E'],   ['F']]
    }

grammar = {
    '<start>' : [['<sentence>']],
    '<sentence>' : [['<noun_phrase>', '<verb>']],
    '<noun_phrase>' : [['<article>', '<noun>']],
    '<noun>' : [['horse'], ['dog'], ['hamster']],
    '<article>' : [['a'], ['the']],
    '<verb>' : [['stands'], ['walks'], ['jumps']]
}

###### SIMPLEST ######

# Performs inverse unification on a given key in a grammar.
# i.e. The final result is a list of strings representing different possible 
# combinations of terminal symbols that could be produced from the specified 
# non-terminal key in the grammar.
def unify_key_inv(grammar, key):
    if key in grammar:
        # We have not reached a terminal symbol yet.
        # Pick a random production associated with the key and repeat.
        return unify_rule_inv(grammar, random.choice(grammar[key]))
    else:
        # If the key is not in the grammar, it must be a terminal symbol.
        # The only string which a terminal symbol can generate is 
        # the symbol itself.
        return [key]
    
def unify_rule_inv(grammar, rule):
    # For each production of the given rule, see what terminal strings can
    # be produced by running `unify_key_inv` on it.
    ls = [unify_key_inv(grammar, token) for token in rule]

    # Flatten the list of lists.
    return sum(ls, [])

# This is the simplest grammar fuzzer in the world.
def simplest_grammar_fuzzer(grammar, key):
    unify_key_inv(grammar, key)


###### MODIFIED TO OBTAIN PARSE TREE ######

# def is_terminal(v):
#     # Based on our grammar, where non-terminals are surrounded by < and >
#     return (v[0], v[-1]) != ('<', '>')

# def is_nonterminal(v):
#     return (v[0], v[-1]) == ('<', '>')

# def tree_to_string(tree):
#     symbol, children, *_ = tree
#     if children:
#         return ''.join(tree_to_string(c) for c in children)
#     else:
#         return '' if is_nonterminal(symbol) else symbol
    
# def unify_key_inv_t(grammar, key):
#     if key in grammar:
#         # Non-terminal case
#         ls = unify_rule_inv_t(grammar, random.choice(grammar[key]))
#         return (key, ls)
#     else:
#         return (key, [])

# def unify_rule_inv_t(grammar, rule):
#     return [unify_key_inv_t(grammar, token) for token in rule]

# def grammar_fuzzer_tree(grammar, key):
#     print(unify_key_inv_t(grammar, '<start>'))

import time

start_time = time.time()

for i in range(10000):
    simplest_grammar_fuzzer(grammar, '<start>')

end_time = time.time()
execution_time = end_time - start_time
print(f"Execution time per run: {execution_time} seconds")





