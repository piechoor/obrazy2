CC = gcc
CFLAGS = -c -Wall

all: przetw_obr

%.o: %.c funkcje.h
	$(CC) $(CFLAGS) $^

przetw_obr: obsluga.o przetwarzanie.o main.o opcje.o
	$(CC) -o $@ $^

clean:
	rm *.o *~