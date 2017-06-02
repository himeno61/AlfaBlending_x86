CC = gcc
CFLAGS = -Wall -m64

all: main.o funkcja.o
	$(CC) $(CFLAGS) -o program main.o funkcja.o `allegro-config --shared`
main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c -m64
funkcja.o: funkcja.asm
	nasm -f elf64 funkcja.asm -o funkcja.o
clean:
	rm -f *.o
	rm program -f
	rm wynik.bmp


