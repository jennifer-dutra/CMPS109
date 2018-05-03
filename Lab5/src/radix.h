/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include <vector>
#include <functional>
#include <iostream>
#include <string>

using namespace std;

/*
 * Simple multi-threaded Radix Sort with support for Most Significant Digit
 * sorting only.
 */
class RadixSort {
public:
    /*
     * Create a multi-threaded RadiX Sort restricted to using no more than
     * CORES processor cores.
     */
    RadixSort(const unsigned int cores);

    /*
     * Perform an in-place Most Significant Digit Radix Sort on each list of
     * unsigned integers in LISTS.
     */
    void msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists);

    /* Functions for radix sort implementation */
    void sort(string *s, string *aux, int lo, int hi, int at, int arraySize);

    void initializeSort(string *s, int arraySize);

    int charAt(string s, int i);

    string* convertToString(unsigned int *arr, int size);

    std::vector<unsigned int> convertToIntVector(string *s, int size);
};
