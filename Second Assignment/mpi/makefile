CFLAGS=-std=c++11

all: primes

primes: mpi.cpp ../papi/papi.cpp
	mpiCC -O2 mpi.cpp ../papi/papi.cpp -lpapi -fopenmp -o mpi

csv-feup:
	make
	echo "" > output.csv
	for p in 16; do \
		for t in 1 2; do \
			for n in 25 26 27 28 29 30 31 32; do \
				ifeq($$t, 1) \
					mpirun --hostfile hostfile -np $$p mpi linear $$n >> output.csv; \
				else \
					for threads in 1 4; do \
						mpirun --hostfile hostfile -np $$p mpi parallel $$threads $$n >> output.csv; \
					done;\
				endif \
			done; \
		done; \
	done; \
