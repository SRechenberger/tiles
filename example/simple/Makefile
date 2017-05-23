test: main.o
	gcc -L../.. -Wall -o test main.o -lncurses -ltiles

main.o: main.c
	gcc -Wall -std=gnu11 -c -o main.o main.c
