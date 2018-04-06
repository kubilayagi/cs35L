Kubilay Agi
UID: 304784519

Initially when I was trying to parallelize the program, I was
trying to use threads for the functions that already existed in 
main.c, such as the one that creates the scene

The issue I had with this was that the function was non-void
and returned an object of type scene_t. I tried making this
a global variable and then running the program, but there were
issues with race condititons because every thread was changing
the scene varaible in its own way, which caused problems with the
program.

Then I realized that the main function itself was likely taking up
the most time because of the double for-loop that was running for the
display, which likely contained lots of pixels.

I moved the majority of the main function (the for-loops and the variable
declarations) to a function that I called get_colors, which I made to be
a funciton that returns a pointer of type void, which is what we need for
using threads on the function.

After running this version, the program was running with the same time
for all of the different numbers of threads, which is not what we expect
to see. This was because each thread was running for the whole program.
To solve this, we needed to have each thread run for only a fraction of
the pixels, and leave the rest of the pixels to the other threads. This
issue was fixed by passing the thread number to the function and using that
as the initial condition for the outer-most for-loop, and I incremented by
the number of threads that the program was running with.

When I ran this the first time, I had #include <pthread.h> but I did not
change the make file and link -lpthread, so I was getting errors where it
said that it did not recognize pthread_create and pthread_join, which was
confusing because I included their library. After re-reading the spec, I
tried to figure out where to link -lpthread, and I discovered that it belongs
in the Makefile. Once in the Makefile, I was not entirely sure where the
flag should be added, so I added it to a few different places without much
success until I added it to the libraries section. I noticed afterward that
there was already another flag in that section that was prefixed with the
letter 'l'.

I was initally concerned that declaring my global array which stored the
pixel colors with variable size would not be allowed, but it ended up
working because the seasnet machine is running C99, which
supports vairable sized arrays on the stack (via StackOverflow).

Below is the results of running the program with the different number
of threads:

time ./srt 1-test.ppm >1-test.ppm.tmp

real 0m45.707s
user 0m45.701s
sys  0m0.002s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real 0m23.067s
user 0m45.862s
sys  0m0.010s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real 0m11.653s
user 0m45.571s
sys  0m0.001s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real 0m5.766s
user 0m44.682s
sys  0m0.001s
mv 8-test.ppm.tmp 8-test.ppm

We can see that the time for each run of the program is
inversely proportional to the number of threads that we
use. Everytime we double the number of threads, the time
is cut in half. Obviously there is a limit in the number
of threads that we use, but if the trend continues, if we
used 32 threads, which is the max that the seasnet server
can handle, the time for that would be around 1.5-2 seconds.

I decided to run the program with 16 and 32 threads to see
if the trend holds up, and these were my results:

time ./srt 16-test.ppm >16-test.ppm.tmp

real 0m3.871s
user 0m54.870s
sys  0m0.003s
mv 16-test.ppm.tmp 16-test.ppm
time ./srt 32-test.ppm >32-test.ppm.tmp

real 0m3.218s
user 1m18.524s
sys  0m0.034s
mv 32-test.ppm.tmp 32-test.ppm

Clearly, the theoretical model does not fit the results
perfectly. From these tests, we can see that the performance
will likely not improve beyond 3 seconds.
