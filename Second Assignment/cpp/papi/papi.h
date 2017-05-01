#ifndef PAPI
#define PAPI

#include <papi.h>

void init_papi();
int init_papi_events();
int stop_papi_events();

#endif
