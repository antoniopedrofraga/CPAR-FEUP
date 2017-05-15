CFLAGS=-std=c++11
libs_for_gcc = -lgnu

all: primes

primes: no_mpi/no_mpi.cpp mpi/mpi.cpp papi/papi.cpp
	mpiCC -O2 mpi/mpi.cpp papi/papi.cpp -lpapi -fopenmp -o mpi/mpi.o
	g++ -O2 no_mpi/no_mpi.cpp papi/papi.cpp -lpapi -fopenmp -o no_mpi/no_mpi.o
clean:
	\rm ./mpi/mpi.o ./no_mpi/no_mpi.o results.csv
csv:
	make
	echo "" > results.csv
	for t in 1 2; do \
		for n in 25 26 27 28 29 30 31 32; do \
			if [ $$t = 1 ] ; then \
				for p in 1 2 4 8 12; do \
					mpirun --hostfile mpi/hosts/hostfile -np $$p mpi/mpi.o linear $$p $$n >> results.csv; \
				done; \
				no_mpi/no_mpi.o linear $$n >> results.csv; \
			else \
				for threads in 1 2 3 4; do \
					for p in 1 2 4 8 12; do \
						mpirun --hostfile mpi/hosts/hostfile -np $$p mpi/mpi.o parallel $$p $$threads $$n >> results.csv; \
					done; \
					no_mpi/no_mpi.o parallel $$threads $$n >> results.csv; \
				done; \
			fi; \
		done; \
	done; \
