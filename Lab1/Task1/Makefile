CC = gcc
FLAGS = -Wall
LIB_NAME = libmemory
SOURCE_NAME = library
MAIN_NAME = main
TEST_NAME = test
static:
	$(CC) $(FLAGS) -c $(SOURCE_NAME).c -o $(LIB_NAME).o
	ar rcs $(LIB_NAME).a $(LIB_NAME).o

shared:
	$(CC) $(FLAGS) -fPIC -c $(SOURCE_NAME).c -o $(LIB_NAME).o
	$(CC) -shared -o $(LIB_NAME).so $(LIB_NAME).o


main_static:
	make static
	$(CC) $(FLAGS) main.c -o $(MAIN_NAME)_static -I ./ $(LIB_NAME).a


main_shared:
	make shared
	unset LD_LIBRARY_PATH
	gcc -L./ -Wl,-rpath=./ -Wall -o $(MAIN_NAME)_shared main.c libmemory.so
	#$(CC) $(FLAGS) -c $(MAIN_NAME).c –$(LIB_NAME) –L ./ -o main

clean:
	rm -f $(MAIN_NAME)_shared $(MAIN_NAME)_static *.so *.o *.a