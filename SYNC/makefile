all : 2_1_1 2_1_2 2_2_1 2_2_2 2_3_1

2_1_1: 2_1_1.c
	gcc -pthread -o 2_1_1 2_1_1.c

2_1_2: 2_1_2.c
	gcc -pthread -o 2_1_2 2_1_2.c

2_2_1: 2_2_1.c
	gcc -pthread -o 2_2_1 2_2_1.c
	
2_2_2: 2_2_2.c
	gcc -pthread -o 2_2_2 2_2_2.c
	
2_3_1: 2_3_1.c
	gcc -pthread -o 2_3_1 2_3_1.c

.PHONY: clean all

clean:
	rm -f *.o *.out *.so
