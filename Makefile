all:
	gcc -Wall -o seq sequencial.c 
run: 
	./seq
	
clean:
	clear
