.PHONY: cook supplier main
cook:
	gcc -Wall cook.c -o cook

supplier:
	gcc -Wall supplier.c -o supplier

main:
	gcc -Wall main.c -o main
	make cook
	make supplier

clean:
	rm -f supplier cook main