''' 
Taken from https://rahul.gopinath.org/post/2019/05/28/simplefuzzer-01/
"The simplest grammar fuzzer in the world"
'''

################################
#                              #
#    Grammar Configuration     #
#                              #
################################

GRAMMAR = {
    0x80 : [[0x81]],
    0x81 : [[0x82, 0x83]],
    0x82 : [[0x84, 0x85]],
    0x85 : [[0x00], [0x01], [0x02]],
    0x84 : [[0x03], [0x04]],
    0x83 : [[0x05], [0x06], [0x07]]
}

START_TOKEN = 0x80
TO_PRINT = False
ITERATIONS = 1

###############################

import random

# Performs inverse unification on a given key in a grammar.
# i.e. The final result is a list of strings representing different possible 
# combinations of terminal symbols that could be produced from the specified 
# non-terminal key in the grammar.
def unify_key_inv(grammar, key):
    if ((key & 0x80) == 0x80):
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
    # Then flatten the list of lists.
    return sum([unify_key_inv(grammar, token) for token in rule], [])

for i in range(ITERATIONS):
    if (TO_PRINT):
        print(unify_key_inv(GRAMMAR, START_TOKEN))
    else:
        unify_key_inv(GRAMMAR, 0x80)