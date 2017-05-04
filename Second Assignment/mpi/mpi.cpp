#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include <string>
#include <omp.h>
#include "../papi/papi.h"
#include <mpi.h>

using namespace std;

const string PARALLEL = "parallel";
const string LINEAR = "linear";
const int ROOT = 0;


long long get_lower_bound(int rank, long long number_of_primes, int size) {
  return ((long long)rank * number_of_primes) / (long long)size;
}
long long  get_higher_bound(int rank, long long number_of_primes, int size) {
  return ((((long long)rank + 1) * number_of_primes) / (long long)size) - 1;
}
long long  get_block_size(long long low_bound, long long high_bound) {
  return high_bound - low_bound;
}

/*Feupinhocomissao2013*/
void sieve_of_eratosthenes_linear(int rank, long long n, int size) {

    long long lower_bound = 2 + get_lower_bound(rank, n, size);
    long long higher_bound = 2 + get_higher_bound(rank, n, size);
    long long block_size = get_block_size(lower_bound, higher_bound);

    vector<bool> prime(block_size, true);

    long long p = 2, j;
    long long limit = sqrt(n);

    while(exp2(p) <= limit) {
        if (exp2(p) < lower_bound) {
      			if (lower_bound % p == 0) { j = lower_bound; }
      			else { j = lower_bound + (p - (lower_bound % p)); }
      	} else {
    			j = exp2(p);
        }
        for (long long i = j; i < higher_bound; i += p) {
			      prime[i - lower_bound] = false;
		    }
    		if (rank == ROOT) {
    			for(long long i = p + 1; i < higher_bound; i++) {
    				if (prime[i - lower_bound]) { p = i; break; }
    			}
        }
        MPI_Bcast(&p, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    }
}
void sieve_of_eratosthenes_parallel(int rank, long long n, int size, int n_threads) {

    long long lower_bound = 2 + get_lower_bound(rank, n, size);
    long long higher_bound = 2 + get_higher_bound(rank, n, size);
    long long block_size = get_block_size(lower_bound, higher_bound);

    vector<bool> prime(block_size, true);

    long long p = 2, j;
    long long limit = sqrt(n);

    while(exp2(p) <= limit) {
        if (exp2(p) < lower_bound) {
      			if (lower_bound % p == 0) { j = lower_bound; }
      			else { j = lower_bound + (p - (lower_bound % p)); }
      	} else {
    			j = exp2(p);
        }
        #pragma omp parallel for num_threads(n_threads)
        for (long long i = j; i < higher_bound; i += p) {
			      prime[i - lower_bound] = false;
		    }
    		if (rank == ROOT) {
    			for(long long i = p + 1; i < higher_bound; i++) {
    				if (prime[i - lower_bound]) { p = i; break; }
    			}
        }
        MPI_Bcast(&p, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    }
}
void init_mpi(int * size, int * rank) {
  MPI::Init();
  MPI_Comm_size(MPI_COMM_WORLD, size);
  MPI_Comm_rank(MPI_COMM_WORLD, rank);
}
int main (int argc, char ** argv) {

  struct timespec start, finish;
  double elapsed;

  vector<string> args;
  stringstream string_stream_n, string_stream_n_threads;
  string run_method;
  unsigned int n, n_threads;
  char time_string[100];
  int ret, size, rank;


  if (argc != 3 && argc != 4) {
    cout << "Please insert the correct number arguments:" << endl;
    cout << "./primes <linear/paralel> (<n_threads>) <exp_of_2>" << endl;
    return -1;
  }

  copy(argv + 1, argv + argc, back_inserter(args));
  run_method = args[0];

  if (!(run_method == PARALLEL || run_method == LINEAR)) {
    cout << "Please define the 1st argument as <linear/parallel>" << endl;
    return -1;
  }

  init_mpi(&size, &rank);
  init_papi_events();
  if (run_method == LINEAR) {
    string_stream_n << args[1];
    string_stream_n >> n;
    if ( rank == ROOT) { clock_gettime(CLOCK_MONOTONIC, &start); }
    sieve_of_eratosthenes_linear(rank, exp2(n), size);
    if ( rank == ROOT) {
      clock_gettime(CLOCK_MONOTONIC, &finish);
    }
  } else if (run_method == PARALLEL) {
    string_stream_n_threads << args[1];
    string_stream_n_threads >> n_threads;
    string_stream_n << args[2];
    string_stream_n >> n;

    if ( rank == ROOT) { clock_gettime(CLOCK_MONOTONIC, &start); }
    sieve_of_eratosthenes_parallel(rank, exp2(n), size, n_threads);
    if ( rank == ROOT) {
      clock_gettime(CLOCK_MONOTONIC, &finish);
    }
  }
  if ( rank == ROOT) {
    elapsed = (finish.tv_sec - start.tv_sec);
  	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
  	sprintf(time_string, "%3.3f", (double)elapsed);
    print_papi_events();
    cout << "mpi," << run_method << "," << n << "," << size << "," << get_l1_dcm() << "," << get_l2_dcm() << "," << time_string << endl << endl;
  }
  stop_papi_events();
  MPI::Finalize();

}
