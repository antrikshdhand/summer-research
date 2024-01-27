# Uniform random sampling of strings 

The goal for this codebase is to be able to randomly sample a string that can be generated from a given grammar. 

We will implement this in three steps:

1. Find the total number of strings of a given size that can be produced from the grammar.
2. Enumerate all the strings that can be produced from the grammar.
3. Pick a specific string given its index in the enumeration.

Once we have the ability to do #2 and #3 we can then uniformly sample
from those indices.

## Progress

Currently implemented the functions `key_get_num_strings()` and `rule_get_num_strings()` to satisfy #1. This can be found in the `naive` implementation.
