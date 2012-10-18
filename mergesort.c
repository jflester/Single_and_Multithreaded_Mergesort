#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <sysexits.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include "mergesort.h"

/* Mergesort.c */

/* Global mutex */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Global variable to reference head of task queue
   Task queue will be a singly-linked list of type Chunk */
Chunk *head;

static void merge(void *starts, void *midd, size_t num_elems, Compare_fn cmp, size_t size_elem);

/* Single-threaded mergesort.
   If arr or cmp are NULL, returns without modifying arr.
   Recursively calls st_mergesort on smaller and smaller halves of the array
   until the array is of size 1, at which point it is automatically sorted.
   On the way back up through the recursion, starts merging larger and larger
   pieces of the array back together using the helper function merge. */
void st_mergesort(void *arr, size_t num_elem, size_t elem_size, Compare_fn cmp){
  void *middle = arr + elem_size*(num_elem/2);
  if (arr == NULL || cmp == NULL){
    return;
  }
  /* If arr has one element, is automatically sorted, so return */
  if (num_elem <= 1){
    return;
  }
  /* Recurse on separate sides, and merge */
  st_mergesort(arr, num_elem/2, elem_size, cmp);
  st_mergesort(middle, num_elem - num_elem/2, elem_size, cmp);
  merge(arr, middle, num_elem, cmp, elem_size);
  /* Return sorted arr */
  return;
}

/* Merge function.
   Used in st_mergesort to merge to arrays together into a single array.
   Copies values over using memcpy. */
void merge(void *starts, void *midd, size_t num_elems, Compare_fn cmp, size_t size_elem){
  char *place = calloc(num_elems, size_elem);
  size_t mid = num_elems/2;
  size_t i = 0, k = 0;
  char *jp = place;
  while ((i < mid) && (k < num_elems - mid)){
    if (cmp(starts+(i*size_elem), midd+(k*size_elem)) <= 0){
      memcpy(jp, starts+(i*size_elem), size_elem);
      i++;
    }
    else {
      memcpy(jp, midd+(k*size_elem), size_elem);
      k++;
    }
    jp += size_elem;
  }
  while (i < mid){
    memcpy(jp, starts+(i*size_elem), size_elem);
    jp += size_elem;
    i++;
  }
  while (k < (num_elems - mid)){
    memcpy(jp, midd+(k*size_elem), size_elem);
    jp += size_elem;
    k++;
  }
  memcpy(starts, place, size_elem*num_elems);
  free(place);
}

/* Struct used to pass args to pthread_create */
struct info {
  void *arr;
  size_t num_elem;
  size_t elem_size;
  Compare_fn cmp;
};

/* Helper function used to call st_mergesort from mt_mergesort */
void *mt_thread(void * arg){
  struct info info;
  if (arg == NULL){
    return NULL;
  }
  info = * (struct info *) arg;
  pthread_mutex_lock(&mutex);
  st_mergesort(info.arr, info.num_elem, info.elem_size, info.cmp);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

/* Multi-threaded mergesort.
   Breaks arr down into chunk_size pieces, and creates num_threads threads to
   check chunks out of a task queue, sort them, and put them back into the
   queue. If they havea buddy who is also done being sorted, then those two
   chunks get merged.
   If arr or cmp is NULL, or if num_threads or chunk_size is too large, will
   just return without modifying arr */
void mt_mergesort(void *arr, size_t num_elem, int chunk_size, size_t elem_size, Compare_fn cmp, int num_threads){
  pthread_t tids[num_threads];
  struct info info[num_threads];
  int i;
  /*void *temp;
    Chunk *curr; */
  if (num_threads < 1 || num_threads > 1024 || chunk_size < 1 || chunk_size > (2^30)){
    return;
  }
  if (arr == NULL || cmp == NULL){
    return;
  }
  /*head = arr;
    head->first = arr; //maybe arr[0]?
    head->size = chunk_size;
    head->next = arr + chunk_size;
    head->done = 0;
    temp = arr;
    curr = head;
    while (temp != NULL){
      temp += arr + chunk_size;
      curr = curr->next;
      curr->size = chunk_size;
      curr->next = temp + chunk_size;
      curr->first = temp;
      curr->done = 0;
   }
  */
  for (i = 0; i < num_threads; i++){
    info[i].arr = (char *)arr + i*((num_elem/chunk_size)*elem_size);
    info[i].num_elem = num_elem/2;
    info[i].elem_size = elem_size;
    info[i].cmp = cmp;
  }
  for(i = 0; i < num_threads; i++) {
    pthread_create(&tids[i], NULL, mt_thread, &info[i]);
  }
  for(i = 0; i < num_threads; i++) {
    pthread_join(tids[i], NULL);
  }
  pthread_mutex_lock(&mutex);
  st_mergesort(arr, num_elem, elem_size, cmp);
  pthread_mutex_unlock(&mutex);
  return;
}