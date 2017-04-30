#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include <string>
#include <papi.h>
#include <omp.h>

using namespace std;

const string PARALLEL = "parallel";
const string LINEAR = "linear";

void sieve_of_eratosthenes_linear(long long n) {
    vector<bool> prime(n-1, true);

    long long p, i;
    long long limit = sqrt(n);

    for (p = 2; p <= limit; p++) {
        if (prime[p] == true) {
            for (i = p * 2; i <= n; i += p)
                prime[i] = false;
        }
    }

}
void sieve_of_eratosthenes_parallel(int n_threads, long long n) {
    vector<bool> prime(n-1, true);

    long long p, i;
    long long limit = sqrt(n);

    #pragma omp parallel for schedule(dynamic) num_threads(n_threads)
    for (p = 2; p <= limit; p++)
        if (prime[p] == true) {
            for (i = p * 2; i <= n; i += p)
                prime[i] = false;
        }
}
int main (int argc, char ** argv) {

  struct timespec start, finish;
  double elapsed;

  vector<string> args;
  stringstream string_stream_n, string_stream_n_threads;
  string run_method;
  unsigned int n, n_threads;
  char time_string[100];


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

  if (run_method == LINEAR) {
    string_stream_n << args[1];
    string_stream_n >> n;

    clock_gettime(CLOCK_MONOTONIC, &start);
    sieve_of_eratosthenes_linear(exp2(n));
    clock_gettime(CLOCK_MONOTONIC, &finish);
  } else if (run_method == PARALLEL) {
    string_stream_n_threads << args[1];
    string_stream_n_threads >> n_threads;
    string_stream_n << args[2];
    string_stream_n >> n;

    clock_gettime(CLOCK_MONOTONIC, &start);
    sieve_of_eratosthenes_parallel(n_threads, exp2(n));
    clock_gettime(CLOCK_MONOTONIC, &finish);
  }

  elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	sprintf(time_string, "Time: %3.3f seconds\n", (double)elapsed);
	cout << time_string;

  cout << endl;
}
