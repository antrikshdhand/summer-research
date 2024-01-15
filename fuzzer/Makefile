CC = gcc
CFLAGS = -Wall -Wextra -O3 -O1
SOURCE_C = src/fuzzer.c
EXECUTABLE_C = out/fuzzer_c.o
PYTHON_SCRIPT = src/fuzzer.py

.PHONY: all c py clean

all: compare

compare: compile_c run_c run_python 

c: compile_c 

py: run_python

clean:
	rm -rf out/*

compile_c:
	$(CC) $(CFLAGS) $(SOURCE_C) -o $(EXECUTABLE_C)

run_python:
	python3 $(PYTHON_SCRIPT)

run_c:
	$(EXECUTABLE_C)