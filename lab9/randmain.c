//    randmain.c

/*
    header files required:
        1. randcpuid.h  :  to check if CPU has RDRAND instruction
        2. stdlib.h     :  allows us to use size_t
        3. stdbool.h    :  lets us use type bool 
        4. errno.h      :  error messages
        5. dlfcn.h      :  lets us use dynamic loading
        6. stdio.h      :  lets us use fprintf
*/ 
 
#include"randcpuid.h"
#include<stdlib.h>
#include<stdbool.h>
#include<errno.h>
#include<dlfcn.h>
#include<stdio.h>

/*
    depending on whether randcpuid reports that the hardware supports
    the RDRAND intruction, main program should:
 
        1. dynamically link hardware-oriented or software oriented
           implementation of randlib
        2. do dynamic linking via dlopen, dlsym
        3. call dlclose to clean up before exiting
        4. if any function called by main program fails, report error and
           exit with nonzero status
*/
 
/* Main program, which outputs N bytes of random data.  */
int main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  //void (*initialize) (void);
  unsigned long long (*rand64) (void);
  char* err;
  void* rlib; // initialize this for dlsym

  // implement dynamic linking
  // dynamically link hardware or software implentation of randlib
  // use dlopen and dlsym
  // dlclose to clean up before exiting
  // error checking
  if (rdrand_supported ())
    {
      rlib = dlopen("./randlibhw.so", RTLD_NOW); 
      // RTLD_NOW: resolve all undefined symbols before dlopen() returns
      if (rlib == NULL)
      {
          fprintf(stderr, "Failure in opening randlibhw.so - error code: %s\n", 
                  dlerror());
          exit(-1);
      }
      rand64 = dlsym(rlib, "hardware_rand64");
      err = dlerror();
      if (err)
      {
          fprintf(stderr, "Failed to locate hardware_rand64 - error code: %s\n",
                  err);
          exit(-1);
      }
    }
  else // software implementation
    {
      rlib = dlopen("./randlibsw.so", RTLD_NOW);
      if (rlib == NULL)
      {
          fprintf(stderr, "Failure in opening randlibsw.so - error code: %s\n",
                  dlerror());
          exit(-1);
      }
      rand64 = dlsym(rlib, "software_rand64");
      err = dlerror();
      if (err)
      {
          fprintf(stderr, "Failed to locate software_rand64 - errr code: %s\n",
                  err);
          exit(-1);
      }
  }

  // initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (fwrite (&x, 1, outbytes, stdout) != outbytes)
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      //finalize ();
      return 1;
    }

  // finalize ();
  dlclose(rlib);

  return 0;
}
