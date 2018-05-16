/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include "radix.h"

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

void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, const unsigned int cores) {
  // note -- changing implementation to more efficient one later
  // this implementation has no errors but only 350% speed up

  std::array<std::vector<unsigned int>, 10> buckets;    // create array of 10 buckets


  for(unsigned int i = 0; i < lists.size(); i++) {

   std::thread threads[buckets.size()];                  // create array of threads
   unsigned int currThreads = 0;                         // current number of threads running
   unsigned int totalThreads = 0;                        // total threads used

   std::vector<unsigned int> currList = lists[i].get();  // copy of current list
   int listSize = lists[i].get().size();                 // size of current list

   // put integers in bucket based on first digit
   // ex. 1000 goes in bucket 1, 2000 goes in bucket 2
   for(int j = 0; j < listSize; j++) {
     int firstDigit = charAt(std::to_string(currList[j]), 0) - 48;  // get first digit
     (buckets.at(firstDigit)).push_back(currList[j]);               // add to correct bucket
   }

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

RadixServer::RadixServer(const int port, const unsigned int cores) {
    // your server implementation goes here :)
}

void RadixClient::msd(const char *hostname, const int port, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) std::cerr << ("open") << std::endl;
    
    struct hostent *server = gethostbyname(hostname);
    if(hostname == NULL) exit(-1);

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(port);

    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      exit(-1);
    }

    // send ints here

    close(sockfd);

}
