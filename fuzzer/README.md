# The world's simplest grammar fuzzer

This repository holds the code for the C implementation of [the simplest grammar fuzzer in the world](https://rahul.gopinath.org/post/2019/05/28/simplefuzzer-01/) originally written by [Dr Rahul Gopinath](https://www.sydney.edu.au/engineering/about/our-people/academic-staff/rahul-gopinath.html). The program generates random strings from a given grammar.

## Getting started

This program follows the C99 standard, and the project uses [`gcc`](https://gcc.gnu.org) for compilation. 

Once you have cloned the repository locally and navigated to the `./fuzzer` directory in your terminal, the included Makefile has several rules for you to use:

1. `make clean`: remove all output files produced in `./out` 
2. `make ccomp`: compiles the `.c` code
3. `make crun`: runs the `.c` code
4. `make py`: runs the `.py` script
5. `make compare`: 
    - `make crun` (does not recompile the `.c` code)
    - `make py` 
    - outputs the `time` information for the programs into `out/c_result.txt` and `out/py_result.txt`

These commands should all work out-of-the-box with the sample grammar provided. 

> Try running `make ccomp` followed by `make compare` and check if the files `./out/c_result.txt` and `./out/py_result.txt` contain bash output. This is a good sign everything has been set up correctly.

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

 



