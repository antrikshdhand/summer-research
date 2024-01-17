# The world's simplest grammar fuzzer

This repository holds the code for the C implementation of [the simplest grammar fuzzer in the world](https://rahul.gopinath.org/post/2019/05/28/simplefuzzer-01/) originally written by [Dr Rahul Gopinath](https://www.sydney.edu.au/engineering/about/our-people/academic-staff/rahul-gopinath.html). The program generates random strings from a given grammar.

## Table of Contents

- [Getting started](#getting-started)
- [About the code](#about-the-code)
    - [Parameters](#parameters)
    - [The 8-bit representation used in C](#the-8-bit-representation-used-in-c)
- [Usage](#usage) 
    - [Importing your grammar](#importing-your-grammar)
    - [Running your fuzzer](#running-your-fuzzer)

## Getting started

This program follows the C99 standard, and the project uses [`gcc`](https://gcc.gnu.org) for compilation. 

Once you have cloned the repository locally and navigated to the `./fuzzer` directory in your terminal, the included Makefile has several rules for you to use:

1. `make clean`: remove all output files produced in `./out` 
2. `make ccomp`: compiles the `fuzzer.c` code
3. `make crun`: runs the `fuzzer_c.o` code
4. `make py`: runs the `fuzzer.py` script
5. `make compare`: 
    - `make crun` (does not recompile the `.c` code)
    - `make py` 
    - outputs the `time` information for the programs into `out/c_result.txt` and `out/py_result.txt`

These commands should all work out-of-the-box with the sample grammar provided. 

> Try running `make ccomp` followed by `make compare` and check if the files `./out/c_result.txt` and `./out/py_result.txt` contain bash output. This is a good sign everything has been set up correctly.

## About the code

### Parameters

The program is reliant on a few parameters to run – these are all declared at the top of the `fuzzer.h` and `fuzzer.py` files.

1. `GRAMMAR`: the grammar you want to generate strings from 
2. `START_TOKEN`: the first non-terminal in the grammar (usually equivalent to '\<start\>' or '\<grammar\>' in BNF form)
2. `TO_PRINT`: whether you want to print the fuzzed strings to the terminal or not
    - Python: toggle between `True` and `False`
    - C: comment and uncomment the preprocessor directive
3. `ITERATIONS`: the number of program iterations you want to run

These are the only parameters you have to change.

For example, if you want to run the fuzzer normally and generate fuzzed strings to the terminal, you would set `TO_PRINT` to `True` and set `ITERATIONS` to `1`. If you want to compare the speed of the Python and C programs, you would set `TO_PRINT` to `False` and `ITERATIONS` to a large number like `100000`, followed by running `make ccomp && make compare`.

Refer to [Importing your grammar](#importing-your-grammar) for more details on how to set up `GRAMMAR`.

### The 8-bit representation used in C

In order to optimise the code as much as possible, we don't work with strings in the C implementation of the fuzzer. Instead, we map each token in the grammar to an 8-bit number and then generate random "strings" from these bytes. This mapping allows us to see some smart optimisations such as O(1) checking if a token is non-terminal or not, and O(1) time index lookup. 

The mapping is simple: nonterminals are assigned 8-bit keys starting from `0x80`, and terminals are assigned 8-bit keys starting from `0x00`. That is, _if the MSB of the key is set, the token is a non-terminal._ 

We also store and assign keys to all non-terminals in the order they appear in the grammar.  For example, in the simple grammar
```
<expression> ::= <term> "+" <term>
<term> ::= <factor> "*" <factor>
<factor> ::= "0" | "1" | "2" | ... | "9"
```


the first non-terminal is `<expression>` and so it would receive the key `0x80`. The second is `<term>` which would receive the key `0x81`, and finally `<factor>` would receive the key `0x82`. We would also store the grammar as such:
```
GRAMMAR = [<NT-struct for 0x80>, <NT-struct for 0x81>, <NT-struct for 0x82>]
```
This allows for the 4 least-significant bits of the key to also provide us with the index of the non-terminal in the grammar.

## Usage

Suppose you have a BNF grammar you wish to work with:

```
<start> ::= <sentence>
<sentence> ::= <noun_phrase> <verb>
<noun_phrase> ::= <article> <noun>
<noun> ::= "horse" | "dog" | "hamster"
<article> ::= "a" | "the"
<verb> ::= "stands" | "walks" | "jumps"
```
<div style="text-align: center"><a href="./data/test_grammar.txt">./data/test_grammar.txt</a></div><br>

This project requires that you have this grammar converted into JSON format:

```json
{
    "<start>": [["<sentence>"]],
    "<sentence>": [["<noun_phrase>", "<verb>"]],
    "<noun_phrase>": [["<article>", "<noun>"]],
    "<noun>": [["horse"], ["dog"], ["hamster"]],
    "<article>": [["a"], ["the"]],
    "<verb>": [["stands"], ["walks"], ["jumps"]]
}
```
<div style="text-align: center"><a href="./data/test_grammar.txt">./data/test_grammar.json</a></div>

### Importing your grammar

We now want to convert the BNF grammar from the above JSON format to the representation we will use in our programs. 

#### Python

We use dictionaries in Python to represent our grammar. Because the JSON format is interchangeable with Python dictionaries, you can simply replace the contents of the `GRAMMAR` variable in `fuzzer.py` with your JSON grammar.

#### C

We use structures in C to represent each part of the Grammar:

```c
typedef uint8_t Token;

typedef struct Rule
{
    size_t num_tokens;
    Token tokens[MAX_TOKENS_IN_RULE];
} Rule;

typedef struct NonTerminal
{
    Token name;
    size_t num_rules;
    Rule rules[MAX_RULES_FOR_NONTERMINAL];
} NonTerminal;

typedef struct Grammar
{
    size_t num_non_terminals;
    NonTerminal non_terminals[MAX_NONTERMINALS_IN_GRAMMAR];
} Grammar;
```
This leads to a more complex initialisation:
```c
Grammar GRAMMAR = {
	// Number of NonTerminals 
    6,

    // Array of NonTerminals
	{
        // NonTerminal 1
		{
            // Name of NonTerminal
			0x80,
   
            // Number of rules NonTerminal has
			1,

            // Array of Rules
			{
                // Rule 1
				{
                    // Number of Tokens in Rule
					1,

                    // Array of Tokens
					{0x81}
				}
			}
		},

        // NonTerminal 2
		{
			0x81,
			1,
			{
				{
					2,
					{0x82, 0x83}
				}
			}
		},
		...
```
Hence we have developed a JSON-to-C converter which takes in a JSON grammar file as a command-line argument and outputs the C initialisation code of the grammar in the above format.

From the `./fuzzer` directory, execute the Python script:
```bash
python3 ./src/converter.py <path_to_json_grammar_file>
```

Replace the contents of `GRAMMAR` in `fuzzer.h` with the outputted code found in `./data/grammar_c_8bit.txt`.

Notes:
- `converter.py` outputs the C initialisation code as well as a lookup table `grammar_lookup.txt` which shows you the keys for every token in the grammar.
- `converter.py` can be run with the `--debug` flag to output a C initialisation code which uses the raw token strings rather than the 8-bit keys.
- We do not read in the JSON and convert it to C dynamically in order to optimise resources. Having the grammar stored in static memory is much faster.

### Running your fuzzer

After you have imported your grammar into both the Python and C programs, you are now able to run all of the Makefile rules explained in the [Getting started](#getting-started) section.

## Contact

Please contact [Dr Rahul Gopinath](https://www.sydney.edu.au/engineering/about/our-people/academic-staff/rahul-gopinath.html) for futher queries.

