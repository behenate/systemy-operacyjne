.PHONY: main
main:
	gcc -Wall -o main main.c
	gcc -Wall -o producer producer.c
	gcc -Wall -o consumer consumer.c
	./main

test:
	gcc -Wall -o testa test.c
	gcc -Wall -o producer producer.c
	gcc -Wall -o consumer consumer.c
	./testa

clean:
	rm -f main consumer procucer producer fifo testa