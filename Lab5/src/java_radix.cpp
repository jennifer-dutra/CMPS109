#include <iostream>
#include <string>

using namespace std;

  int R = 2<<8;

  void initializeSort(string s[], int arraySize);
  void sort(string s[], string aux[], int lo, int hi, int at, int arraySize);
  int charAt(string s[], int i);

  void initializeSort(string s[], int arraySize) {
      string aux[arraySize];
      int lo = 0;
      int hi = arraySize - 1;
      int at = 0;
      sort(s, aux, lo, hi, at, arraySize);
  }

  int charAt(string s, int i){
      if(i < s.length()) return s.at(i);
      else return -1;
  }

  void sort(string s[], string aux[], int lo, int hi, int at, int arraySize){

      if(hi <= lo) return;

      int *count;
      count = new int[R+2];

      for(int i = lo; i <= hi; ++i)    count[charAt(s[i], at)+2]++;

      for(int i = 0; i < R+1; ++i)     count[i+1] += count[i];

      for(int i = lo; i <= hi; ++i)    aux[count[charAt(s[i], at)+1]++] = s[i];

      for(int i = lo; i <= hi; ++i)    s[i] = aux[i-lo];

      for(int r=0; r<R; ++r) sort(s, aux, lo+count[r], lo+count[r+1]-1, at+1, arraySize);
  }

  int main(int argc, char *argv[]) {

      // string s[] = {"a", "aa", "ab", "abc", "d", "qq", "ff"};
      string s[] = { "33", "54", "3", "135", "644", "3", "5", "13", "53", "502", "99" };

      int arraySize = sizeof(s) / sizeof(string);
      initializeSort(s, arraySize);
      for(int i = 0; i < arraySize; ++i) std::cout << s[i] << std::endl;

  }
