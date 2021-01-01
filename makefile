program: obsluga.o przetwarzanie.o main.o
	gcc -o a.out obsluga.o przetwarzanie.o main.o
	rm *.o

obsluga.o: obsluga.c
	gcc -c -Wall obsluga.c

przetwarzanie.o: przetwarzanie.c
	gcc -c -Wall przetwarzanie.c

main.o: main.c 
	gcc -c -Wall main.c
