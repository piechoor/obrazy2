funkcje.o: funkcje.c
	gcc -c -Wall funkcje.c

main.o: main.c 
	gcc -c -Wall main.c

program: funkcje.o main.o
	gcc -o a.out funkcje.o main.o

clean:
	rm *.o
	rm a.out