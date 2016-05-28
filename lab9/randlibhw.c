//    randlibhw.c

#include<randlib.h>
#include<imintrin.h> // from randall.c

/*
    1. contain hardware implementation of random number generator
    2. implement interface described by randlib.h
*/

extern unsigned long long hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}










