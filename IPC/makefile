all:code.out

code.out: codeIPC.o
	gcc -Wall -o code.out codeIPC.o

codeIPC.o: codeIPC.c codeIPC.h
	gcc -Wall -c codeIPC.c

.PHONY: clean all

clean:
	rm -f *.o *.out *.so
