# Uniform random sampling of strings 

This repository holds code for the C implementation of [Uniform Random Sampling of Strings from Context-Free Grammar](https://rahul.gopinath.org/post/2021/07/27/random-sampling-from-context-free-grammar/#a-memoized-implementation), originally written by [Dr Rahul Gopinath](https://www.sydney.edu.au/engineering/about/our-people/academic-staff/rahul-gopinath.html). 

## Purpose

The goal is to be able to randomly sample a string that can be generated from a given grammar.  The algorithm will implement this in three steps:

1. Find the total number of strings of a given size that can be produced from the grammar.
2. Enumerate all the strings that can be produced from the grammar.
3. Pick a specific string given its index in the enumeration.

Once we have the ability to do #2 and #3 we can then uniformly sample from those indices.

## Progress

I am currently working on implementing the memoised implementation of the algorithm (see [A Memoised Implementation.](https://rahul.gopinath.org/post/2021/07/27/random-sampling-from-context-free-grammar/#a-memoized-implementation)).

This has a few parts:
1. Two main functions to populate the grammar inside the memoisation data structure.
2. Helper functions which build upon the aforementioned data structure to provide:
    - Key and rule counts
    - Valid strings from keys and rules 
    - The ability to randomly access any particular string
3. The culmination of the entire codebase to randomly sample a string from the grammar.

I have currently completed Part 1.

