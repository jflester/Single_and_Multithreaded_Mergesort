<h4>MERGESORT.C</h4>
Runs the mergesort algorithm on arrays, either in a singlethreaded fashion or multithreaded.

In the multithreaded code, there is a block comment for partial set up of a task queue to
do the multithreading - however that approach was not working, so the threads were set up
in a more traditional manner.
<h4>BRESOURCE.H & RESOURCE.H</h4>
Files from the *GNU C Library* for use in mergesort.
<h4>MERGETEST.C</h4>
Can be used to test results from **MERGESORT.C** using the single and multithreaded functions
and see how long it takes each method to accomplish a task given a random of array of a size
provided by the user.
<h4>TEST.C</h4>
Is an even simpler test file to make sure single threaded in **MERGESORT.C** doesn't change the length
of the given array.
<h4>MAKEFILE</h4>
Traditional makefile, using **gcc** as the compiler.