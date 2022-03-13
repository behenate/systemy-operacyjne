CC = gcc
FLAGS = -Wall
LIB_NAME = libmemory
SOURCE_NAME = library
MAIN_NAME = main
STATIC_TEST_NAME = test
TASK_1_PATH = ../Task1
TASK_2_PATH = .
TASK_3_PATH = ../Task3
# .PHONY: <typyt> jak coś się może nie wykryć zmiana pliku

.PHONY: main test
static:
	$(CC) $(FLAGS) -c $(TASK_1_PATH)/$(SOURCE_NAME).c -o $(TASK_1_PATH)/$(LIB_NAME).o
	ar rcs $(TASK_1_PATH)/$(LIB_NAME).a $(TASK_1_PATH)/$(LIB_NAME).o

main:
	make static;
	$(CC) $(FLAGS) $(TASK_2_PATH)/main.c -o $(TASK_2_PATH)/$(MAIN_NAME) -I ./ $(TASK_1_PATH)/$(LIB_NAME).a
test:
	make main;
	./main ;
