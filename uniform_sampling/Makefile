CC = gcc
CFLAGS = -O3 
EXECUTABLE_C = out/uniform_sample.o

default: ; # Options: make ccomp, make crun, make clean

ccomp: 
	$(CC) $(CFLAGS) -c ../fuzzer/src/c/fuzzer.c -o ../fuzzer/out/fuzzer.o
	$(CC) $(CFLAGS) -c ./src/sample.c -o ./out/sample.o
	$(CC) $(CFLAGS) -c ./src/hash_table.c -o ./out/hash_table.o
	$(CC) $(CFLAGS) ./out/sample.o ./out/hash_table.o ../fuzzer/out/fuzzer.o -o $(EXECUTABLE_C)

crun:
	$(EXECUTABLE_C)

clean:
	rm -rf out/*