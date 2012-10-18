#include <stdio.h>
#include "mergesort.h"


static int cmp(const void * a, const void * b) {
  int *x = (int *) a;
  int *y = (int *) b;
  printf("calling cmp\n");
  if ( *x > *y) return 1;
  if (*x == *y) return 0;
  return -1;
}

int main() {
  int original[] = {2,1};
  int sorted[] = {1,2};
  int i;
  Compare_fn my_cmp = cmp;
  printf("about to sort\n");
  st_mergesort(&original[0], sizeof(original)/sizeof(*original), sizeof(*original),
	    my_cmp);

  for (i = 0; i < sizeof(original)/sizeof(*original); i++) {
    if (original[i] == sorted[i]) {
      printf("CORRECT: ");
    }
    else {
      printf("INCORRECT: ");
    }
    printf("After sorting, arr[%d] == %d, should be %d\n",
	   i, original[i], sorted[i]);
  }

  return 0;
}