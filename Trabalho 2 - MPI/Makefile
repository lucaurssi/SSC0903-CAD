IMG = 1000
PAR = parallel
NP  = 2

all:
	gcc -Wall -o seq sequential.c
	mpicc $(PAR).c -o par -Wall

run: 
	./seq $(IMG)
	mpirun -np $(NP) --hostfile hosts-halley ./par $(IMG)

local:
	mpirun -np $(NP) ./par $(IMG)
