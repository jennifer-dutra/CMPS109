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


  // create array of 10 buckets
  std::array<std::vector<unsigned int>, 10> buckets;

  std::thread threads[buckets.size()];

  for(unsigned int i = 0; i < lists.size(); i++) {

    // get current list and list size
    std::vector<unsigned int> currList = lists[i].get();
    int listSize = lists[i].get().size();

    // put integers in bucket based on first digit
    // ex. 1000 goes in bucket 1, 2000 goes in bucket 2
    for(int j = 0; j < listSize; j++) {
      int firstDigit = charAt(std::to_string(currList[j]), 0) - 48;  // get first digit
      (buckets.at(firstDigit)).push_back(currList[j]);               // add to correct bucket
    }

    // sort each bucket
    for(unsigned int j = 1; j < buckets.size(); j++) {

      threads[j] = (std::thread(sortArray, std::ref(buckets.at(j)), buckets.at(j).size()));

    }

    // join remaining threads
    for(unsigned int j = 1; j <  buckets.size(); j++) {
      threads[j].join();
    }

    // clear original vector
    lists[i].get().clear();

    // merge sorted vectors
    for(unsigned int k = 0; k < buckets.size(); k++) {
      for(unsigned int j = 0; j < buckets.at(k).size(); j++) {
        lists[i].get().push_back(buckets.at(k).at(j));
      }
      buckets.at(k).clear();
    }
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
