/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

// radix sort
#include <vector>
#include <functional>
#include <thread>

// client - server
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// need for client
#include <netdb.h>




/*
 * Parallel Radix Sort with support for Most Significant Digit sorting only.
 *
 * If allowed to use ten cores or more, should sort a large set of randomly
 * selected unsigned integers apromixmately ten times faster than a single
 * threaded sort of the same random set.
 *
 * Whilst the choice of sorting algorithnm is a matter or personal choice,
 * a bucket sort where each bucket is sorted in a different thread will almost
 * certainly deliver the best results.
 */
class ParallelRadixSort {
public:
    /*
     * Perform an in-place Most Significant Digit Radix Sort on each list of
     * unsigned integers in LISTS using nore that CORES cpu cores.
     */
    void msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, const unsigned int cores);
};

class RadixServer {
public:
    RadixServer(const int port, const unsigned int cores);
};

class RadixClient {
public:
    void msd(const char *hostname, const int port, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists);
};
