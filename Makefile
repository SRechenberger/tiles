example/test: example/main.o build/frame.o
	gcc -Wall -o example/test example/main.o build/frame.o -lncurses

example/main.o: example/main.c src/frame.h
	gcc -Wall -std=gnu11 -c -o example/main.o example/main.c

build/frame.o: src/frame.c src/frame.h
	gcc -Wall -std=gnu11 -c -o build/frame.o src/frame.c
