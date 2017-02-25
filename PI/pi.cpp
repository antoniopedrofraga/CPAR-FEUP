#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <cstdlib>

#include <iostream>
#include <iomanip>

using namespace std;

int main (int argc, char *argv[])
{
	clock_t tStart, tFinal;
	char st[100];

	double area, pi, x;
	int i, n;
	char c;
	
	cout << "Numero de processadores: " << omp_get_num_procs() << endl;

	cout << "Numero de divisoes ? ";
	cin >> n; 
	area = 0.0;

	tStart = clock();

	//#pragma omp parallel for private(x) reduction(+:area)
	for (i = 0; i < n; i++) {
	   x = (i+0.5)/n;
	   area += 4.0/(1.0 + x*x);
	}
	pi = area / n;

	tFinal = clock();

	cout << setprecision(18) << "PI = " << pi << endl << endl;
	sprintf(st, "Time: %3.3f seconds\n", (double)(tFinal - tStart) / CLOCKS_PER_SEC);
	cout << st;
	
	cout << "Enter para continuar ...";
	cin.get(c);
	cin.get(c);
	
}
