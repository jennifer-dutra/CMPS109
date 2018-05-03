/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include "radix.h"

// global variable needed for radix sort
int R = 2<<8;

int RadixSort::charAt(string s, int i){

    if((unsigned int)i < s.length()) return s.at(i);
    else return -1;
}

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

void RadixSort::initializeSort(string *s, int arraySize) {
    string *aux;
    aux = new string[arraySize];

    int lo = 0;
    int hi = arraySize - 1;
    unsigned int at = 0;
    sort(s, aux, lo, hi, at, arraySize);
}

string* RadixSort::convertToString(unsigned int *arr, int size) {

    string *s;
    s = new string[size];

    for(int i = 0; i < size; i++) {
      s[i] = to_string(arr[i]);
    }
    return s;
}

unsigned int* RadixSort::convertToInt(string *s, int size) {

    unsigned int *arr;
    arr = new unsigned int[size];

    for(int i = 0; i < size; i++) {
      arr[i] = atoi(s[i].c_str());
    }
    return arr;
}

RadixSort::RadixSort(const unsigned int cores) {
    // int ununsed = 0; // to fail complier warming test, remove
}

void RadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
    // your implementation goes here :)

    unsigned int s[] = {33, 54, 3, 135, 644, 3, 5, 13, 53, 502, 99};

    int arraySize = sizeof(s) / sizeof(unsigned int);

    string* stringNums = convertToString(s, arraySize);

    initializeSort(stringNums, arraySize);

    unsigned int* sorted = convertToInt(stringNums, arraySize);

    for(int i = 0; i < arraySize; ++i) std::cout << sorted[i] << std::endl;

    // int size = 0;
    //
    // for (unsigned int i = 0; i < lists.size(); i++) {
    //   // size += lists[i].get().size();
    //
    //   size++;
    // }
    //
    // std::cout << "Size: " << size;

}
