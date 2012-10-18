mergesort.c

Runs the mergesort algorithm on arrays, either in a singlethreaded fashion or multithreaded.

In the multithreaded code, there is a block comment for partial set up of a task queue to
do the multithreading - however that approach was not working, so the threads were set up
in a more traditional manner.



bresource.h & resource.h

Files from the GNU C Library for use in mergesort.



mergetest.c

Can be used to test results from mergesort.c using the single and multithreaded functions
and see how long it takes each method to accomplish a task given a random of array of a size
provided by the user.



test.c

Is an even simpler test file to make sure single threaded in mergesort.c doesn't change the length
of the given array.



makefile

Traditional makefile, using gcc as the compiler.