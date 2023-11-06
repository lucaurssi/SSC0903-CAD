all:
	gcc -Wall -o seq sequencial.c -fopenmp
	
run: 
	./seq
	
clean:
	clear
