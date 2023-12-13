IMG = 1000
np=2

all:
	gcc -Wall -o seq sequential.c
run: 
	./seq $(IMG)
	
par:
	mpicc par.c -o par -Wall

run_par:
	mpirun -np $(np) ./par $(IMG)
