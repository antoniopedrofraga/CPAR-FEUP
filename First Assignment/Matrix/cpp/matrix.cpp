#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <papi.h>
#include <omp.h>

using namespace std;


void OnMult(int m_ar, int m_br, int n_threads)
{

	struct timespec start, finish;
	double elapsed;

	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;



    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for(i = 0; i < m_ar; i++)
		for(j = 0; j < m_ar; j++)
			pha[i * m_ar + j] = (double)1.0;



	for (i = 0; i < m_br; i++)
		for (j = 0; j < m_br; j++)
			phb[i * m_br + j] = (double)(i + 1);



    	clock_gettime(CLOCK_MONOTONIC, &start);

	#pragma omp parallel for private(i, j, k) reduction(+:temp) num_threads(n_threads)
	for(i = 0; i < m_ar; i++)
		for(j = 0; j < m_br; j++) {
			temp = 0;
			for(k = 0; k < m_ar; k++) {
				temp += pha[i*m_ar+k] * phb[k*m_br+j];
			}
			phc[i * m_ar + j] = temp;
		}



    	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	sprintf(st, "Time: %3.3f seconds\n", (double)elapsed);
	cout << st;

	cout << "Rsesult matrix: " << endl;
	for (i = 0; i < 1; i++) {
		for (j = 0; j < min(10, m_br); j++)
			cout << phc[j] << " ";
	}
	cout << endl;

    free(pha);
    free(phb);
    free(phc);
}


void OnMultLine(int m_ar, int m_br, int n_threads)
{
    	struct timespec start, finish;
	double elapsed;

	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;



    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for(i = 0; i < m_ar; i++)
		for(j = 0; j < m_ar; j++)
			pha[i * m_ar + j] = (double)1.0;



	for (i = 0; i < m_br; i++)
		for (j = 0; j < m_br; j++)
			phb[i * m_br + j] = (double)(i + 1);


    clock_gettime(CLOCK_MONOTONIC, &start);

	#pragma omp parallel for private(i, j, k) num_threads(n_threads)
	for(i = 0; i < m_ar; i++) {
		for(j = 0; j < m_br; j++) {
			for(k = 0; k < m_ar; k++) {
				phc[i*m_ar+k] += pha[i*m_ar+j] * phb[j*m_br+k];
			}
		}
	}


    	clock_gettime(CLOCK_MONOTONIC, &finish);


	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	sprintf(st, "Time: %3.3f seconds\n", (double)elapsed);
	cout << st;

	cout << "Result matrix: " << endl;
	for(i = 0; i < 1; i++){
		for(j = 0; j < min(10,m_br); j++)
			cout << phc[j] << " ";
	}
	cout << endl;

    free(pha);
    free(phb);
    free(phc);

}

void handle_error(int retval) {
  printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
  exit(1);
}

void init_papi() {
  int retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT && retval < 0) {
    printf("PAPI library version mismatch!\n");
    exit(1);
  }
  if (retval < 0) handle_error(retval);

  std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
            << " MINOR: " << PAPI_VERSION_MINOR(retval)
            << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}


int main (int argc, char *argv[]) {

	char c;
	int lin, col, nt=1, n_threads;
	int op;

	int EventSet = PAPI_NULL;
  	long long values[2];
  	int ret;

	ret = PAPI_library_init( PAPI_VER_CURRENT );
	if ( ret != PAPI_VER_CURRENT )
		std::cout << "FAIL" << endl;


	ret = PAPI_create_eventset(&EventSet);
		if (ret != PAPI_OK) cout << "ERRO: create eventset" << endl;


	ret = PAPI_add_event(EventSet,PAPI_L1_DCM );
	if (ret != PAPI_OK) cout << "ERRO: PAPI_L1_DCM" << endl;


	ret = PAPI_add_event(EventSet,PAPI_L2_DCM);
	if (ret != PAPI_OK) cout << "ERRO: PAPI_L2_DCM" << endl;

	op = 1;
	do {
		cout << endl << "1. Multiplication" << endl;
		cout << "2. Line Multiplication" << endl;
		cout << "Selection?: ";
		cin >>op;
		cout << endl;
		if (op == 0)
			break;
		printf("Dimensions: lins cols ? ");
   		cin >> lin >> col;

		printf("Number of threads (0 to automatic)? ");
   		cin >> n_threads;



		// Start counting
		ret = PAPI_start(EventSet);
		if (ret != PAPI_OK) cout << "ERRO: Start PAPI" << endl;

		switch (op){
			case 1:
				OnMult(lin, col, n_threads);
				break;
			case 2:
				OnMultLine(lin, col, n_threads);
				break;
		}

  		ret = PAPI_stop(EventSet, values);
  		if (ret != PAPI_OK) cout << "ERRO: Stop PAPI" << endl;
  		printf("L1 DCM: %lld \n",values[0]);
  		printf("L2 DCM: %lld \n",values[1]);

		ret = PAPI_reset(EventSet);
		if (ret != PAPI_OK)
			std::cout << "FAIL reset" << endl;



	} while (op != 0);

		ret = PAPI_remove_event( EventSet, PAPI_L1_DCM );
		if ( ret != PAPI_OK )
			std::cout << "FAIL remove event" << endl;

		ret = PAPI_remove_event( EventSet, PAPI_L2_DCM );
		if ( ret != PAPI_OK )
			std::cout << "FAIL remove event" << endl;

		ret = PAPI_destroy_eventset( &EventSet );
		if ( ret != PAPI_OK )
			std::cout << "FAIL destroy" << endl;

}
