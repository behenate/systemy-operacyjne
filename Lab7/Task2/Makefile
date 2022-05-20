.PHONY: cook supplier main
cook:
	gcc -Wall cook.c -o cook  -lrt

supplier:
	gcc -Wall supplier.c -o supplier  -lrt

main:
	gcc -Wall main.c -o main  -lrt
	make cook
	make supplier

clean:
	rm -f supplier cook main