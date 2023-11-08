IMG = 10000

all:
	gcc -Wall -o seq sequencial.c -fopenmp
	gcc -Wall -o par paralelo.c -fopenmp
run: 
	./seq $(IMG)
	./par $(IMG) 1
	./par $(IMG) 2
	./par $(IMG) 4
	./par $(IMG) 6
	./par $(IMG) 8
	./par $(IMG) 10
	./par $(IMG) 12
	./par $(IMG) 14
	./par $(IMG) 16
	
clean:
	clear
