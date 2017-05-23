mapgen: main.o
	gcc -L../.. -Wall -o mapgen main.o -lncurses -ltiles

main.o: main.c
	gcc -Wall -std=gnu11 -c -o main.o main.c
