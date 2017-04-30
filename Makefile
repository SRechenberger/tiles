INSTALL_DIR=/usr/local
LIB_DIR=$(INSTALL_DIR)/lib
HEADER_DIR=$(INSTALL_DIR)/include/libtiles

HEADER_FILES=src/*.h
OBJECT_FILES=*.o

libtiles.so.0.0.2: frame.o
	gcc -shared -Wl,-soname,libtiles.so.0 -o libtiles.so.0.0.2 $(OBJECT_FILES)

frame.o: src/frame.c src/frame.h
	gcc -Wall -std=gnu11 -c -fPIC -o frame.o src/frame.c

# If this method to install the library seems dangerous to you, please tell me.
install: libtiles.so.0.0.2
	cp libtiles.so.0.0.2 $(LIB_DIR)
	ldconfig -v -n $(LIB_DIR)
	ln -sf $(LIB_DIR)/libtiles.so.0 $(LIB_DIR)/libtiles.so
	mkdir $(HEADER_DIR) || true
	cp $(HEADER_FILES) $(HEADER_DIR)

clean:
	rm *.o *.so*
