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

// global variable needed for radix sort
int R = 2<<8;

// gets character at specified index in string
int RadixSort::charAt(string s, int i){
    if((unsigned int)i < s.length()) return s.at(i);
    else return -1;
}

// sorts array
void RadixSort::sort(string *s, string *aux, int lo, int hi, int at, int arraySize){

    if(hi <= lo) return;

    int *count;
    count = new int[R+2];

    for(int i = lo; i <= hi; ++i)    count[charAt(s[i], at)+2]++;

    for(int i = 0; i < R+1; ++i)     count[i+1] += count[i];

    for(int i = lo; i <= hi; ++i)    aux[count[charAt(s[i], at)+1]++] = s[i];

    for(int i = lo; i <= hi; ++i)    s[i] = aux[i-lo];

    for(int r=0; r<R; ++r) sort(s, aux, lo+count[r], lo+count[r+1]-1, at+1, arraySize);
}

// initializes variables for radix sort
void RadixSort::initializeSort(string *s, int arraySize) {
    string *aux;
    aux = new string[arraySize];

    int lo = 0;
    int hi = arraySize - 1;
    unsigned int at = 0;
    sort(s, aux, lo, hi, at, arraySize);
}

// converts array of integers to array of strings
string* RadixSort::convertToString(unsigned int *arr, int size) {
    string *s;
    s = new string[size];

    for(int i = 0; i < size; i++) {
      s[i] = to_string(arr[i]);
    }
    return s;
}

// converts array of strings to vector of unsigned ints
std::vector<unsigned int> RadixSort::convertToIntVector(string *s, int size) {
    std::vector<unsigned int> vector;

    for(int i = 0; i < size; i++) {
       vector.push_back(atoi(s[i].c_str()));
    }
    return vector;
}

RadixSort::RadixSort(const unsigned int cores) {
    // int ununsed = 0; // to fail complier warming test, remove
}

void RadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {

    for(unsigned i = 0; i < 4; i++) {

      std::vector<unsigned int> v = lists[i].get();

      unsigned int* s = &v[i];

      // int arraySize = sizeof(s) / sizeof(unsigned int); // NEED TO GET RIGHT ARRAY SIZE

      int arraySize = 200;

      string* stringNums = convertToString(s, arraySize);

      initializeSort(stringNums, arraySize);

      std::vector<unsigned int> sorted = convertToIntVector(stringNums, arraySize);

      lists[i].get() = sorted;

      // PRINT TESTING
      for(int i=0; i < arraySize; ++i) {
        cout << sorted[i] << endl;
      }
    }


    // int size = 0;
    //
    // for (unsigned int i = 0; i < lists.size(); i++) {
    //   size += lists[i].get().size();
    //   size++;
    // }
    //
    // std::cout << "Size: " << size;

}
