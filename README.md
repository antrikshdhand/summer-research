# 2023/24 Summer Research Internship

This repository holds work related to my current research internship at USYD. My project centres around comparing blackbox grammar inference algorithms.

```
summer-research/
├── fuzzer/
│   ├── data/
│   │   ├── test_grammar.txt
│   │   └── test_grammar.json
│   ├── out/
│   │   └── fuzzer_c.o  
│   ├── src/
│   │   ├── c/
│   │   │   ├── fuzzer.c
│   │   │   ├── fuzzer.h
│   │   │   ├── grammar.h
│   │   │   └── main.c 
│   │   ├── converter.py
│   │   └── fuzzer.py
│   ├── Makefile
│   └── README.md
├── uniform_sampling/
│   ├── out
│   └── src/
│       ├── memoised/
│       │   ├── grammar_hash_table.c
│       │   ├── key_hash_table.c
│       │   ├── memoised.c
│       │   ├── memoised.h
│       │   └── rule_hash_table.c
│       └── naive/
│           ├── hash_table.c
│           ├── hash_table.h
│           └── naive.c
├── .gitignore
└── README.md
```