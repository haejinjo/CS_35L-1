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

   rm -f *.o *.tmp 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm srt srt.tgz
   gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o main.o main.c
   gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o raymath.o raymath.c
   gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o shaders.o shaders.c
   gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter -o srt main.o raymath.o shaders.o -lm -lpthread
   time ./srt 1-test.ppm >1-test.ppm.tmp

   real    0m57.856s
   user    0m57.828s
   sys     0m0.023s
   mv 1-test.ppm.tmp 1-test.ppm
   time ./srt 2-test.ppm >2-test.ppm.tmp
   ./srt: Multithreading is not supported yet.

   real    0m0.002s
   user    0m0.000s
   sys     0m0.001s
   Makefile:36: recipe for target '2-test.ppm' failed
   make: *** [2-test.ppm] Error 1

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

3. To incorporate multithreading into the program, I took the four for loops
   out of the main routine and put that into a separate function called process.
   I had to change the condition of the outer for loop to correspond to the
   thread number that was passed into the function.

   This function was then called within the main routine based on the number
   of threads that are specified. That way, the original process could be
   achieved across multiple threads, without having to go through the entire
   process fro top to bottom. I created these threads using the function
   pthread_create and passed in the process function as one of the arguments.
   
4. Some problems that I encountered when trying to implement the multithreading
   were figuring out the arguments to pass the pthread_create. In particular, 
   I had trouble with determining when to pass a pointer or an actual thread to
   these functions. In addition, when I made the variables above global, I
   forgot to take out the declarations in the main routine and/or the
   multithreaded function, so this resulted in an infinite loop. This problem
   took awhile to resolve because I was looking for more explicit logic errors
   and not something as trivial as a declaration that I failed to remove.

   After I made all the appropriate changes, I tested the multithreaded
   performance and ran make check clean again. The output is as follows:

   rm -f *.o *.tmp 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm srt srt.tgz
   gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter    -c -o main.o main.c
   gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter    -c -o raymath.o raymath.c
   gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter    -c -o shaders.o shaders.c
   gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter  -o srt main.o raymath.o shaders.o -lm -lpthread
   time ./srt 1-test.ppm >1-test.ppm.tmp

   real    0m59.938s
   user    0m59.920s
   sys     0m0.007s
   mv 1-test.ppm.tmp 1-test.ppm
   time ./srt 2-test.ppm >2-test.ppm.tmp

   real    0m31.174s
   user    1m0.751s
   sys     0m0.001s
   mv 2-test.ppm.tmp 2-test.ppm
   time ./srt 4-test.ppm >4-test.ppm.tmp

   real    0m14.229s
   user    0m56.205s
   sys     0m0.001s
   mv 4-test.ppm.tmp 4-test.ppm
   time ./srt 8-test.ppm >8-test.ppm.tmp

   real    0m10.280s
   user    1m2.301s
   sys     0m0.002s
   mv 8-test.ppm.tmp 8-test.ppm
   for file in 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm; do \
   diff -u baseline.ppm $file || exit; \
   done


   Final thoughts:
   It's clear that multithreading made the program far more efficient. We see
   that with 8 threads, the real time is about one-sixth of the time that
   it took one thread to complete execution. As mentioned in the lab portion
   of the assignment, if we continue to increase the number of threads, then
   we will see that the rate of improvement will slow down and eventually
   plateau. There is a clear advantage to using multhreading to boost the
   efficiency and speed of this program.

