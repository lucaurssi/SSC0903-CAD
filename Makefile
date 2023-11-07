all:
	gcc -Wall -o seq sequencial.c -fopenmp
	gcc -Wall -o par paralelo.c -fopenmp
run: 
	./seq
	./par 1
	./par 4
	
clean:
	clear
