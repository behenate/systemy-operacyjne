CC = gcc
FLAGS = -Wall
LIB_NAME = libmemory
SOURCE_NAME = library
MAIN_NAME = main
STATIC_TEST_NAME = test
SHARED_TEST_NAME = test_shared
DYNAMIC_TEST_NAME = test_dynamic
TASK_1_PATH = ../Task1
TASK_2_PATH = ../Task2
TASK_3_PATH = .
# .PHONY: <typyt> jak coś się może nie wykryć zmiana pliku

.PHONY: test test_dynamic test_shared test_static optimization_test
static:
	$(CC) $(FLAGS) -c $(TASK_1_PATH)/$(SOURCE_NAME).c -o $(TASK_1_PATH)/$(LIB_NAME).o
	ar rcs $(TASK_1_PATH)/$(LIB_NAME).a $(TASK_1_PATH)/$(LIB_NAME).o

shared:
	$(CC) $(FLAGS) -fPIC -c $(TASK_1_PATH)/$(SOURCE_NAME).c -o $(TASK_1_PATH)/$(LIB_NAME).o
	$(CC) $(FLAGS) -shared -o $(TASK_1_PATH)/$(LIB_NAME).so $(TASK_1_PATH)/$(LIB_NAME).o


test_static:
	make static;
	$(CC) $(FLAGS) $(TASK_2_PATH)/main.c -o $(TASK_3_PATH)/$(STATIC_TEST_NAME) -I ./ $(TASK_1_PATH)/$(LIB_NAME).a

test_shared:
	unset LD_LIBRARY_PATH
	gcc -L$(TASK_1_PATH) -Wl,-rpath=$(TASK_1_PATH) -Wall -o $(SHARED_TEST_NAME) $(TASK_2_PATH)/main.c $(TASK_1_PATH)/libmemory.so

test_dynamic:
	make shared
	$(CC) ../Task2/main_dynamic.c -o $(DYNAMIC_TEST_NAME) -ldl
test:
	echo "---------------------------------------- TEST STATIC LIBRARY: ----------------------------------------" > $(TASK_3_PATH)/results3a.txt;
	echo "" >> $(TASK_3_PATH)/results3a.txt;
	make test_static
	./$(STATIC_TEST_NAME) >> results3a.txt
	echo "---------------------------------------- TEST SHARED LIBRARY: ----------------------------------------" >> $(TASK_3_PATH)/results3a.txt;
	echo "" >> $(TASK_3_PATH)/results3a.txt;
	make test_shared
	./$(SHARED_TEST_NAME) >> results3a.txt
	make test_dynamic
	echo "---------------------------------------- TEST DYNAMIC LIBRARY: ----------------------------------------" >> $(TASK_3_PATH)/results3a.txt;
	./$(DYNAMIC_TEST_NAME) >> results3a.txt


# ------------------------------ TASK 3B ----------------------------------------------------------------------------
test_static_O1:
	$(CC) $(FLAGS) $(TASK_2_PATH)/main.c -o $(TASK_3_PATH)/$(STATIC_TEST_NAME)_01 -I ./ $(TASK_1_PATH)/$(LIB_NAME).a -O1
	echo "Static O1: " >> $(TASK_3_PATH)/results3b.txt
	./$(TASK_3_PATH)/$(STATIC_TEST_NAME)_01 >>$(TASK_3_PATH)/results3b.txt

test_shared_01:
	unset LD_LIBRARY_PATH
	gcc -L$(TASK_1_PATH) -Wl,-rpath=$(TASK_1_PATH) -Wall -o $(SHARED_TEST_NAME)_01 $(TASK_2_PATH)/main.c $(TASK_1_PATH)/libmemory.so -O1
	echo "Shared O1: " >> $(TASK_3_PATH)/results3b.txt
	./$(TASK_3_PATH)/$(SHARED_TEST_NAME)_01 >>$(TASK_3_PATH)/results3b.txt

test_dynamic_01:
	$(CC) $(FLAGS) ../Task2/main_dynamic.c -o $(DYNAMIC_TEST_NAME)_01 -ldl -O1
	echo "Dynamic O1: " >> $(TASK_3_PATH)/results3b.txt
	./$(TASK_3_PATH)/$(DYNAMIC_TEST_NAME)_01 >>$(TASK_3_PATH)/results3b.txt

test_static_O3:
	$(CC) $(FLAGS) $(TASK_2_PATH)/main.c -o $(TASK_3_PATH)/$(STATIC_TEST_NAME)_03 -I ./ $(TASK_1_PATH)/$(LIB_NAME).a -O3
	echo "Static O3: " >> $(TASK_3_PATH)/results3b.txt
	./$(TASK_3_PATH)/$(STATIC_TEST_NAME)_03 >>$(TASK_3_PATH)/results3b.txt

test_shared_03:
	unset LD_LIBRARY_PATH
	gcc -L$(TASK_1_PATH) -Wl,-rpath=$(TASK_1_PATH) -Wall -o $(SHARED_TEST_NAME)_03 $(TASK_2_PATH)/main.c $(TASK_1_PATH)/libmemory.so -O3
	echo "Shared O3: " >> $(TASK_3_PATH)/results3b.txt
	./$(TASK_3_PATH)/$(SHARED_TEST_NAME)_03 >>$(TASK_3_PATH)/results3b.txt

test_dynamic_03:
	$(CC) $(FLAGS) ../Task2/main_dynamic.c -o $(DYNAMIC_TEST_NAME)_03 -ldl -O3
	echo "Dynamic O3: " >> $(TASK_3_PATH)/results3b.txt
	./$(TASK_3_PATH)/$(DYNAMIC_TEST_NAME)_03 >>$(TASK_3_PATH)/results3b.txt


test_static_Os:
	$(CC) $(FLAGS) $(TASK_2_PATH)/main.c -o $(TASK_3_PATH)/$(STATIC_TEST_NAME)_Os -I ./ $(TASK_1_PATH)/$(LIB_NAME).a -Os
	echo "Static Os: " >> $(TASK_3_PATH)/results3b.txt
	$(TASK_3_PATH)/$(STATIC_TEST_NAME)_Os >>$(TASK_3_PATH)/results3b.txt

test_shared_0s:
	unset LD_LIBRARY_PATH
	gcc -L$(TASK_1_PATH) -Wl,-rpath=$(TASK_1_PATH) -Wall -o $(SHARED_TEST_NAME)_Os $(TASK_2_PATH)/main.c $(TASK_1_PATH)/libmemory.so -Os
	echo "Shared Os: " >> $(TASK_3_PATH)/results3b.txt
	$(TASK_3_PATH)/$(SHARED_TEST_NAME)_Os >>$(TASK_3_PATH)/results3b.txt

test_dynamic_0s:
	$(CC) ../Task2/main_dynamic.c -o $(DYNAMIC_TEST_NAME)_Os -ldl -Os
	echo "Dynamic Os: " >> $(TASK_3_PATH)/results3b.txt
	$(TASK_3_PATH)/$(DYNAMIC_TEST_NAME)_Os >>$(TASK_3_PATH)/results3b.txt

optimization_test:
	make static;
	make shared;
	echo "---------------------------------------- TEST OPTIMIZATION O1: ----------------------------------------" > $(TASK_3_PATH)/results3b.txt;
	make test_static_O1;
	make test_shared_01;
	make test_dynamic_01;
	echo "---------------------------------------- TEST OPTIMIZATION O3: ----------------------------------------" >> $(TASK_3_PATH)/results3b.txt;
	make test_static_O3;
	make test_shared_03;
	make test_dynamic_03;
	make test_dynamic
	echo "---------------------------------------- TEST OPTIMIZATION Os: ----------------------------------------" >> $(TASK_3_PATH)/results3b.txt;
	make test_static_Os;
	make test_shared_0s;
	make test_dynamic_0s;

