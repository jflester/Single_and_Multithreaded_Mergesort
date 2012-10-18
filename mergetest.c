#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <sysexits.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>
#include <pthread.h>
#include "mergesort.h"

/* Mergetest.c */

/* Used to test st_mergesort and mt_mergesort from mergesort.c
   Meaures and displays the time taken to run each with a randomly
   generated array of integers.
   Displays wall time, user time, and sys time taken. */

/* Comparision function used to pass to st_mergesort and mt_mergesort. */
static int cmp(const void * a, const void * b) {
  int *x = (int *) a;
  int *y = (int *) b;

  if (*x > *y) return 1;
  if (*x == *y) return 0;
  return -1;
}

int main(int argc, char *argv[]) {
  int num, seed, up_bound, i, j, *arr, *temp, z;
  struct rusage usage1, usage2;
  struct timeval wall1, wall2;
  
  if (argc < 4) {
    printf("need three args to run this mergetest\n");
    return 1;
  }
  num = atoi(argv[1]);
  z = atoi(argv[2]);
  seed = atoi(argv[3]);
  up_bound = atoi(argv[4]);
  arr = malloc(sizeof(int)*num);
  if (arr == NULL) {
    fprintf(stderr, "malloc failed\n");
    return -1;
  }
  temp = malloc(sizeof(int)*num);
  if (temp == NULL) {
    fprintf(stderr, "second malloc failed\n");
    return -1;
  }

  srand(seed);
  for (i = 0; i < num; i++) {
    arr[i] = rand()%up_bound;
  }
  for (i = 0; i <= 5; i++) {
    for (j = 0; j < num; j++) {
      temp[j] = arr[j];
    }
    printf("%d threads: ", i);
    gettimeofday(&wall1, NULL);
    getrusage(RUSAGE_SELF, &usage1);
    if (!i) {
      st_mergesort(temp, num, sizeof(int), cmp);
    }
    else {
      mt_mergesort(temp, num, z, sizeof(int), cmp, i);
    }
    gettimeofday(&wall2, NULL);
    getrusage(RUSAGE_SELF, &usage2);
    wall2.tv_sec -= wall1.tv_sec;
    if ((wall2.tv_usec -= wall1.tv_usec) < 0) {
      wall2.tv_usec += 1000000;
      wall2.tv_sec--;
    }
    usage2.ru_utime.tv_sec -= usage1.ru_utime.tv_sec;
    if ((usage2.ru_utime.tv_usec -= usage1.ru_utime.tv_usec) < 0) {
      usage2.ru_utime.tv_usec += 1000000;
      usage2.ru_utime.tv_sec--;
    }
    usage2.ru_stime.tv_sec -= usage1.ru_stime.tv_sec;
    if ((usage2.ru_stime.tv_usec -= usage1.ru_stime.tv_usec) < 0) {
      usage2.ru_stime.tv_usec += 1000000;
      usage2.ru_stime.tv_sec--;
    }
    printf("%d.%06ds wall; %d.%06ds user; %d.%06ds sys\n",
	   (int) wall2.tv_sec, (int) wall2.tv_usec,
	   (int) usage2.ru_utime.tv_sec, (int) usage2.ru_utime.tv_usec,
	   (int) usage2.ru_stime.tv_sec, (int) usage2.ru_stime.tv_usec);
  }
  free(arr);
  free(temp);
  return 0;
}