all: program

obsluga.o: obsluga.c
	gcc -c -Wall obsluga.c

przetwarzanie.o: przetwarzanie.c
	gcc -c -Wall przetwarzanie.c

main.o: main.c 
	gcc -c -Wall main.c

opcje.o: opcje.c
	gcc -c -Wall opcje.c

program: obsluga.o przetwarzanie.o main.o opcje.o
	gcc -o a.out obsluga.o przetwarzanie.o main.o opcje.o

clean:
	rm *.o *~
