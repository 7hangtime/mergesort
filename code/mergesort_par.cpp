#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <vector>
// I removed chrono because I'll be using sbatch on Centaurus to time execution, so just decluttered
#include "omp_tasking.hpp"

#define DEBUG 0

using namespace tasking;

// just set this to what it said in the project description
size_t threshold = 1000;

void generateMergeSortData (std::vector<int>& arr, size_t n) {
  for (size_t  i=0; i< n; ++i) {
    arr[i] = rand();
  }
}
  
void checkMergeSortResult (std::vector<int>& arr, size_t n) {
  bool ok = true;
  for (size_t  i=1; i<n; ++i)
    if (arr[i]< arr[i-1])
      ok = false;
  if(!ok)
    std::cerr<<"notok"<<std::endl;
}


void merge(int * arr, size_t  l, size_t  mid, size_t r, int* temp) {
  
#if DEBUG
  std::cout<<l<<" "<<mid<<" "<<r<<std::endl;
#endif

  // short circuits
  if (l == r) return;
  if (r-l == 1) {
    if (arr[l] > arr[r]) {
      size_t temp = arr[l];
      arr[l] = arr[r];
      arr[r] = temp;
    }
    return;
  }

  size_t i, j, k;
  size_t n = mid - l;
  
  // init temp arrays
  for (i=0; i<n; ++i)
    temp[i] = arr[l+i];

  i = 0;    // temp left half
  j = mid;  // right half
  k = l;    // write to 

  // merge
  while (i<n && j<=r) {
     if (temp[i] <= arr[j] ) {
       arr[k++] = temp[i++];
     } else {
       arr[k++] = arr[j++];
     }
  }

  // exhaust temp 
  while (i<n) {
    arr[k++] = temp[i++];
  }
}


void mergesort(int * arr, size_t l, size_t r, int* temp) {
  if (l < r) {
    size_t mid = (l+r)/2; // splitting array in half
    size_t len = r - l + 1; // size of the segment

    if (len > threshold) { // Checking to see if it's actualy worth parallelizing
      // left half
      taskstart([&]() {
        mergesort(arr, l, mid, temp);
      });
      // right half
      taskstart([&]() {
        mergesort(arr, mid+1, r, temp);
      });

      // wait for both child tasks before merging
      taskwait();
    } else { // If it's less than the threshold, uses the sequential code provided
      mergesort(arr, l, mid, temp);
      mergesort(arr, mid+1, r, temp);
    }

    merge(arr, l, mid+1, r, temp + l); // puts it back togatha
  }
}


int main (int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> [nbthreads]"<<std::endl;
    return -1;
  }
  
  // command line parameter
  size_t n = atol(argv[1]);

  // pulls threads from cmdline
  int nbthreads = 1;
  if (argc >= 3) {
    nbthreads = atoi(argv[2]);
    if (nbthreads < 1) nbthreads = 1;
  }

  // get arr data
  std::vector<int> arr (n);
  generateMergeSortData (arr, n);

#if DEBUG
  for (size_t i=0; i<n; ++i) 
    std::cout<<arr[i]<<" ";
  std::cout<<std::endl;
#endif

  std::vector<int> temp (n);

  // sort in parallel region
  doinparallel([&]() {
    mergesort(&(arr[0]), 0, n-1, &(temp[0]));
  }, nbthreads);

  // check correctness
  checkMergeSortResult (arr, n);

#if DEBUG
  for (size_t i=0; i<n; ++i) 
    std::cout<<arr[i]<<" ";
  std::cout<<std::endl;
#endif

  return 0;
}
