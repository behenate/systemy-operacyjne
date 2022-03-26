CC = gcc
FLAGS = -Wall
LIB_NAME = task1
SOURCE_NAME = task1
MAIN_NAME = main
TEST_NAME = main_time_test
static:
	gcc -Wall -c $(SOURCE_NAME).c -o $(LIB_NAME).o
	ar rcs $(LIB_NAME).a $(LIB_NAME).o


main:
	make static
	$(CC) $(FLAGS) main.c -o $(MAIN_NAME) -I ./ $(LIB_NAME).a

test:
	make static
	$(CC) $(FLAGS) $(TEST_NAME).c -o $(TEST_NAME) -I ./ $(LIB_NAME).a
	./$(TEST_NAME)

clean:
	rm -f $(MAIN_NAME) $(MAIN_NAME)_static *.so *.o *.a $(TEST_NAME) test1_out.txt large_out.txt