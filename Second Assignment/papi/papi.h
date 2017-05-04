#ifndef PAPI
#define PAPI

#include <papi.h>

void init_papi();
int init_papi_events();
int print_papi_events();
int stop_papi_events();
long long get_l1_dcm();
long long get_l2_dcm();

#endif
