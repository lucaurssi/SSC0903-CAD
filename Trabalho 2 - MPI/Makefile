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

relatorio:
	./seq $(IMG)
	mpirun -np 2 --hostfile hosts-halley ./par $(IMG)
	mpirun -np 4 --hostfile hosts-halley ./par $(IMG)
	mpirun -np 6 --hostfile hosts-halley ./par $(IMG)
	mpirun -np 8 --hostfile hosts-halley ./par $(IMG)
	mpirun -np 10 --hostfile hosts-halley ./par $(IMG)
	mpirun -np 20 --hostfile hosts-halley ./par $(IMG)
	mpirun -np 32 --hostfile hosts-halley ./par $(IMG)
	mpirun -np 40 --hostfile hosts-halley ./par $(IMG)