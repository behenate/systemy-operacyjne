main:
	gcc -o task2 main.c task2.c -I. -lm

test:
	gcc -o test time_test.c task2.c -I. -lm
	./test
	make clean

clean:
	rm -f w*.txt
.PHONY: test