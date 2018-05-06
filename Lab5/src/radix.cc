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


RadixSort::RadixSort(const unsigned int cores) {

  // declare variable for max number of cores
  maxCores = cores;

}

void RadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {

    std::thread threads[lists.size()];

    unsigned int threadCount = 0;               // current number of threads

    unsigned int maxCores = (*this).maxCores;   // max CPU cores used

    for(unsigned int i = 0; i < lists.size(); i++) {

      int listSize = lists[i].get().size();     // number of elements in each list

      // if threadCount is less than maxCores then continue to create threads
      if(threadCount < maxCores) {
        threads[threadCount] = (std::thread(sortArray, lists[i], listSize));
        threadCount++;
      }
      // if max number of cores in use, join all threads
      else {
        for(unsigned int i = 0; i < maxCores; i++) {
          threads[i].join();
        }
        // reset thread count
        threadCount = 0;

        // create thread that was skipped over because max cores was reached
        threads[threadCount] = (std::thread(sortArray, lists[i], listSize));
        threadCount++;
      }
    }

    // join remaining threads
    for(unsigned int i = 0; i < threadCount; i++) {
      threads[i].join();
    }
}
