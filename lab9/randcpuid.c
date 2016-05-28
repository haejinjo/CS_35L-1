//    randcpuid.c

#include"randcpuid.h"
#include<cpuid.h>    // included from randall.c

/*
    1. determines whether current CPU has RDRAND instruction
    2. implement interface described by randcpuid.h
*/


/* Description of the current CPU.  */
struct cpuid { unsigned eax, ebx, ecx, edx; };

/* Return information about the CPU.  See <http://wiki.osdev.org/CPUID>.  */
static struct cpuid
cpuid (unsigned int leaf, unsigned int subleaf)
{
  struct cpuid result;
  asm ("cpuid"
       : "=a" (result.eax), "=b" (result.ebx),
	 "=c" (result.ecx), "=d" (result.edx)
       : "a" (leaf), "c" (subleaf));
  return result;
}



/* Return true if the CPU supports the RDRAND instruction.  */
// change to extern
extern _Bool rdrand_supported (void)
{
  struct cpuid extended = cpuid (1, 0);
  return (extended.ecx & bit_RDRND) != 0;
}

// DONE
