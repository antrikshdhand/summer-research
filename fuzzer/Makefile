CC = gcc
CFLAGS = -O3 
EXECUTABLE_C = out/simplest_fuzzer.o
PYTHON_SCRIPT = src/fuzzer.py

.PHONY: default ccomp py run_c_with_output run_py_with_output clean

default: ; # Options: make ccomp, make crun, make py, make compare

compare: run_c_with_output run_py_with_output

crun:
	$(EXECUTABLE_C)

ccomp: 
	$(CC) $(CFLAGS) -c ./src/c/fuzzer.c -o ./out/fuzzer.o
	$(CC) $(CFLAGS) -c ./src/c/main.c -o ./out/main.o
	$(CC) $(CFLAGS) ./out/main.o ./out/fuzzer.o -o ./out/simplest_fuzzer.o

py: 
	python3 $(PYTHON_SCRIPT)

clean:
	rm -rf out/*

run_c_with_output:
	{ time $(EXECUTABLE_C) ; } 2> ./out/c_time.txt

run_py_with_output:
	{ time python3 $(PYTHON_SCRIPT) ; } 2> ./out/py_time.txt
