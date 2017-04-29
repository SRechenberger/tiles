test: main.o frame.o
	gcc -Wall -o test main.o frame.o -lncurses

main.o: main.c frame.h
	gcc -Wall -std=gnu11 -c -o main.o main.c

frame.o: frame.c frame.h
	gcc -Wall -std=gnu11 -c -o frame.o frame.c
