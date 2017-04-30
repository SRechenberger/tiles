libtiles.so.0.0.2: frame.o
	gcc -shared -Wl,-soname,libtiles.so.0 -o libtiles.so.0.0.2 frame.o

frame.o: src/frame.c src/frame.h
	gcc -Wall -std=gnu11 -c -fPIC -o frame.o src/frame.c

# If this method to install the library seems dangerous to you, please tell me.
install: libtiles.so.0.0.2
	cp libtiles.so.0.0.2 /usr/local/lib
	ldconfig -v -n /usr/local/lib
	ln -sf /usr/local/lib/libtiles.so.0 /usr/local/lib/libtiles.so
	mkdir /usr/local/include/tiles || true
	cp src/frame.h /usr/local/include/tiles

clean:
	rm *.o *.so*
