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


  std::vector<std::vector<unsigned int>> buckets(10);

  std::vector<std::vector<unsigned int>> splitBuckets;


  std::thread listThread[lists.size()];


  for(unsigned int i = 0; i < lists.size(); i++) {


    unsigned int currThreads = 0;                         // current number of threads running
    unsigned int totalThreads = 0;                        // total threads used

    std::vector<unsigned int> currList = lists[i].get();  // copy of current list
    int listSize = lists[i].get().size();                 // size of current list

    // put integers in bucket based on first digit
    // ex. 1000 goes in bucket 1, 2000 goes in bucket 2
    for(int j = 0; j < listSize; j++) {
      int firstDigit = charAt(std::to_string(currList.at(j)), 0) - 48;  // get first digit
      (buckets.at(firstDigit)).push_back(currList.at(j));               // add to correct bucket
    }

    // find the smallest bucket size
    unsigned int smallestBucket = buckets.at(1).size();
    for(unsigned int j = 1; j < buckets.size(); j++) {
      if(buckets.at(j).size() < smallestBucket && smallestBucket != 0) {
        smallestBucket = buckets.at(j).size();
      }
    }
    std::cout << "smallest bucket: " << smallestBucket << std::endl;

    // divide buckets
    float bucketDivide;
    for(unsigned int j = 1; j < buckets.size(); j++) {
      if(smallestBucket < buckets.at(j).size()) {
        bucketDivide = (float)buckets.at(j).size() / smallestBucket;
        bucketDivide = floor(bucketDivide);
        std::cout << "divide bucket by: " << bucketDivide << std::endl;
      }

      // create sub buckets based on size compared to smallest bucket
      // Code for making sub vectors from: https://wandbox.org/permlink/XYTiDZpOUiom1xMc

      if(bucketDivide > 1) {
        int bunch_size = bucketDivide;
    	  std::vector<std::vector<unsigned int>> sub;
    	  sub.reserve((buckets.at(j).size() + 1) /  bunch_size);

      	for(size_t k = 0; k < buckets.at(j).size(); k += bunch_size) {
      		auto last = std::min(buckets.at(j).size(), k + bunch_size);
      		sub.emplace_back(buckets.at(j).begin() + k, buckets.at(j).begin() + last);
      	}

        for(unsigned int k = 0; k < sub.size(); k++) {
          splitBuckets.push_back(sub.at(k));
        }
      }
    }


    std::thread threads[buckets.size()];                  // create array of threads

    // sort each bucket
    for(unsigned int j = 1; j < buckets.size(); j++) {

      threads[currThreads] = (std::thread(sortArray, std::ref(buckets.at(j)), buckets.at(j).size()));
      currThreads++;
      totalThreads++;

      if(currThreads == cores || totalThreads ==  buckets.size() - 1) {
        for(unsigned int k = 0; k < currThreads; k++) {
          threads[k].join();
        }
        currThreads = 0;
      }
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
}
