CC = gcc
CFLAGS = -O3
SOURCE_C = src/fuzzer.c
EXECUTABLE_C = out/fuzzer_c.o
PYTHON_SCRIPT = src/fuzzer.py
C_RESULT = out/c_time.txt
PY_RESULT = out/py_time.txt

compare: run_c_with_output run_py_with_output

crun: 
	$(EXECUTABLE_C)

ccomp: 
	$(CC) $(CFLAGS) $(SOURCE_C) -o $(EXECUTABLE_C)

py: 
	python3 $(PYTHON_SCRIPT)

clean:
	rm -rf out/*

run_c_with_output:
	{ time $(EXECUTABLE_C) ; } 2> ./out/c_result.txt

run_py_with_output:
	{ time python3 $(PYTHON_SCRIPT) ; } 2> ./out/py_result.txt