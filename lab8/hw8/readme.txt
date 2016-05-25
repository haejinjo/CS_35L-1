### Assignment 9
### readme.txt
### Eric Chuu

### brief after-action report of homework
### discuss any issues that you ran into
### conclusions about how well my implementation of SRT improves performance


1. In order to use POSIX threads, I included <pthread.h> in main.c and
   included the link to the lpthread in the Makefile. I then ran the command
   make clean check to get a sense of what was happening. The execution took a
   long time, and as expected, there were messages that said that multithreading
   was not supported yet.

   I took a look at main.c and saw that in the main routine, there was a section
   of code with four nested for loops that went through the pixels. As seen in
   the output of the times above, this process clearly takes a long
   time. Therefore, I concluded that by using a multithreaded implementation, we
   could make this process more efficient.

2. I first took out the if statement in the main checking for nthreads not equal
   to one, as I wanted to incorproate mulitple threads to improve performance.
   Then, I made was creating some global variables:
   nthreads
   scene
   image_vals

   nthreads needed to be made global because it is used to see how many threads
   we want to create before the multithreading begins, as well as being used in
   the multhreaded function.

   scene needed to be made global because it is used in the multithreaded
   function, and it is used before the multithreading begins.

   image_vals needed to be global because it stores the pixel colors in the
   multithreaded function and is eventually used in the main routine to print
   each of the pixels out.

