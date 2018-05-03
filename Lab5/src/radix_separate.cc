#include <iostream>
#include <string>

/*
 * Radix sort code based off of java implementation at :
 * http://www.codebytes.in/2015/12/msd-string-sort-java.html
 * Converted to C++ implementation.
 *
 */

using namespace std;

  int R = 2<<8;

  void initializeSort(string *s, int arraySize);
  void sort(string *s, string *aux, int lo, int hi, int at, int arraySize);
  int charAt(string s, int i);
  string* convertToString(unsigned int *arr, int size);
  unsigned int* convertToInt(string *s, int size);

  void initializeSort(string *s, int arraySize) {
      string *aux;
      aux = new string[arraySize];

      int lo = 0;
      int hi = arraySize - 1;
      int at = 0;
      sort(s, aux, lo, hi, at, arraySize);
  }

  int charAt(string s, int i){
      if(i < s.length()) return s.at(i);
      else return -1;
  }

  void sort(string *s, string *aux, int lo, int hi, int at, int arraySize){

      if(hi <= lo) return;

      int *count;
      count = new int[R+2];

      for(int i = lo; i <= hi; ++i)    count[charAt(s[i], at)+2]++;

      for(int i = 0; i < R+1; ++i)     count[i+1] += count[i];

      for(int i = lo; i <= hi; ++i)    aux[count[charAt(s[i], at)+1]++] = s[i];

      for(int i = lo; i <= hi; ++i)    s[i] = aux[i-lo];

      for(int r=0; r<R; ++r) sort(s, aux, lo+count[r], lo+count[r+1]-1, at+1, arraySize);
  }

  string* convertToString(unsigned int *arr, int size) {

    string *s;
    s = new string[size];

    for(int i = 0; i < size; i++) {
      s[i] = to_string(arr[i]);
    }
    return s;
  }

  unsigned int* convertToInt(string *s, int size) {

    unsigned int *arr;
    arr = new unsigned int[size];

    for(int i = 0; i < size; i++) {
      arr[i] = atoi(s[i].c_str());
    }
    return arr;
  }

  int main(int argc, char *argv[]) {

      unsigned int s[] = {33, 54, 3, 135, 644, 3, 5, 13, 53, 502, 99};

      int arraySize = sizeof(s) / sizeof(unsigned int);

      string* stringNums = convertToString(s, arraySize);

      initializeSort(stringNums, arraySize);

      unsigned int* sorted = convertToInt(stringNums, arraySize);

      for(int i = 0; i < arraySize; ++i) std::cout << sorted[i] << std::endl;

  }
