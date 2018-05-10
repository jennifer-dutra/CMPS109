/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include "radix.h"

/*
 * Radix sort code based off of java implementation at :
 * http://www.codebytes.in/2015/12/msd-string-sort-java.html
 * Converted to C++ implementation.
 *
 * Conversion help from webcast:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=bdacac7d-79ee-4ed5-a25b-6f1f3af9cb27
 */

// global variable for sorting
static int R = 2<<8;

static int charAt(std::string s, int i) {
   if((unsigned int)i < s.length())
     return s.at(i);
   else return -1;
}


static void sort(unsigned int s[], unsigned int aux[], int lo, int hi, int at) {

   if(hi <= lo)
     return;

   int count[R+2];

   for(int i = 0; i < R + 2; i++)
     count[i] = 0;

   for(int i = lo; i <= hi; ++i)
     count[charAt(std::to_string(s[i]), at)+2]++;

   for(int i = 0; i < R+1; ++i)
     count[i+1] += count[i];

   for(int i = lo; i <= hi; ++i)
     aux[count[charAt(std::to_string(s[i]), at)+1]++] = s[i];

   for(int i = lo; i <= hi; ++i)
     s[i] = aux[i-lo];

   for(int r = 0; r < R; ++r)
     sort(s, aux, lo+count[r], lo+count[r+1]-1, at+1);
}

static void sortArray(std::vector<unsigned int> &vec, int len) {
   unsigned int* s = &vec[0];
   unsigned int aux[len];
   int lo = 0;
   int hi = len - 1;
   int at = 0;
   sort(s, aux, lo, hi, at);
}


void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, unsigned int cores) {

  std::vector<unsigned int> firstList = lists[0].get();

  int listSize = lists[0].get().size();

  int bucketSize = listSize / 9;

  // create array of 10 buckets
  std::array<std::vector<unsigned int>, 10> buckets;

  // put integers in bucket based on first
  for(int i = 0; i < listSize; i++) {
    int firstDigit = charAt(std::to_string(firstList[i]), 0) - 48;  // get first digit
    (buckets.at(firstDigit)).push_back(firstList[i]);
  }

  //sort each bucket

  for(unsigned int i = 1; i < buckets.size(); i++) {
    sortArray(buckets.at(i), bucketSize);
  }

  // sortArray(buckets.at(2), bucketSize);

  for(int i = 0; i < bucketSize; i++) {
    std::cout << buckets.at(2)[i] << std::endl;
  }


  // std::thread threads[lists.size()];
  //
  // unsigned int threadCount = 0;               // current number of threads
  //
  // for(unsigned int i = 0; i < lists.size(); i++) {
  //
  //   int listSize = lists[i].get().size();     // number of elements in each list
  //
  //   // if threadCount is less than maxCores then continue to create threads
  //   threads[threadCount] = (std::thread(sortArray, lists[i], listSize));
  //   threadCount++;
  //
  //   if(threadCount == cores) {
  //     for(unsigned int i = 0; i < cores; i++) {
  //       threads[i].join();
  //     }
  //     threadCount = 0;    // reset thread count
  //   }
  // }
  //
  // // join remaining threads
  // for(unsigned int i = 0; i < threadCount; i++) {
  //   threads[i].join();
  // }
}
