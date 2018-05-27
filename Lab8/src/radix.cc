/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include "radix.h"


/*
 * Radix sort code based off of java implementation at:
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

/*
 * Referenced class notes for datagram socket server & client:
 * https://classes.soe.ucsc.edu/cmps109/Spring18/SECURE/13.Distributed3.pdf
 *
 */

/*
 * Start a UDP listener on PORT and accept lists of unsiged integers from
 * clients to be MSD RAdix sorted using no more that CORES cpu cores. After
 * sorting the lists are to be retiurned to the client.
 */
void RadixServer::start(const int port, const unsigned int cores) {

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) exit(-1);

    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
      exit(-1);

    struct sockaddr_in remote_addr;
    socklen_t len = sizeof(remote_addr);

    Message msg;
    std::vector<std::reference_wrapper<std::vector<unsigned int>>> lists;
    std::vector<unsigned int> current;

    msg.num_values = 0;
    msg.sequence = 0;

    unsigned int packet = 0;
    std::vector<unsigned int> missing;
    bool allReceived = false;

    while(true) {
      recvfrom(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, &len);
      msg.num_values = ntohl(msg.num_values);
      msg.sequence = ntohl(msg.sequence);
      msg.flag = ntohl(msg.flag);

      // std::cout << "packet: " << packet << "msg seq: " << msg.sequence << std::endl;

      // check for missing packets and add to vector
      if(packet != msg.sequence && allReceived == false) {
        missing.push_back(packet);
        // std::cout << "push back: " << packet << std::endl;
        packet++;
      }
      packet++;

      for(unsigned int i = 0; i < msg.num_values; i++) {
        current.push_back(ntohl(msg.values[i]));
      }

      if(msg.flag == LAST) {

        // get missing packets
        if(missing.size() > 0 && allReceived == false) {
          allReceived = true;
          msg.num_values = 0;
          for(unsigned int &missingNum : missing) {
            msg.values[msg.num_values++] = missing[missingNum];
          }

          // send missing packet numbers
          msg.num_values = htonl(msg.num_values);
          msg.sequence = htonl(msg.sequence);
          msg.flag = htonl(RESEND);
          sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, len);
          continue;
        }

        // all packets have been received including resends
        allReceived = true;

        //sort
        lists.push_back(std::ref(current));
        ParallelRadixSort serverSort;
        serverSort.msd(lists, cores);

        msg.num_values = 0;
        msg.sequence = 0;
        msg.flag = NONE;


        for(unsigned int j = 0; j < lists[0].get().size(); j++) {
          msg.values[msg.num_values++] = htonl(lists[0].get().at(j));

          if(msg.num_values == MAX_VALUES) {
            msg.num_values = htonl(msg.num_values);
            msg.sequence = htonl(msg.sequence);
            msg.flag = htonl(NONE);
            sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, len);
            msg.sequence = ntohl(msg.sequence);
            msg.sequence++;
            msg.num_values = 0;
          }
        }
        msg.num_values = htonl(msg.num_values);
        msg.sequence = htonl(msg.sequence);
        msg.flag = htonl(LAST);
        sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, len);
        lists[0].get().clear();
      }
    }
    close(sockfd);
}

/*
 * Shutdown the server. Typically this will involve closing the server socket.
 */
void RadixServer::stop() {

}

/*
 * Send list from client to the server. Receives sorted packets back and adds
 * to original vector. 
 */

void msdHelper(Message msg, std::vector<unsigned int> &currList, int listSize, socklen_t len, int sockfd,
   struct sockaddr_in remote_addr, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, unsigned int i, bool resendAll) {

  msg.num_values = 0;
  msg.sequence = 0;
  msg.flag = NONE;

  // send all numbers for the current list
  for(int j = 0; j < listSize; j++) {
    msg.values[msg.num_values++] = htonl(currList.at(j));
    if(msg.num_values == MAX_VALUES) {
      msg.num_values = htonl(msg.num_values);
      msg.sequence = htonl(msg.sequence);
      msg.flag = htonl(NONE);
      sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, len);
      msg.sequence = ntohl(msg.sequence);
      msg.sequence++;
      msg.num_values = 0;
    }
  }
  msg.num_values = htonl(msg.num_values);
  msg.sequence = htonl(msg.sequence);

  msg.flag = htonl(LAST);
  sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, len);
  msg.sequence = ntohl(msg.sequence);

  //clear original vector
  lists[i].get().clear();

  uint packets = 0;

  // recieve all numbers in sorted order
  do {
    recvfrom(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, &len);
    msg.sequence = ntohl(msg.sequence);
    msg.flag = ntohl(msg.flag);

    // if we have recieved last packet and something is missing, resend all
    if(packets != msg.sequence && resendAll == false && msg.flag == LAST) {
      resendAll = true; // set flag so only one recursive call possible
      msdHelper(msg, currList, listSize, len, sockfd, remote_addr, lists, i, resendAll);
    }
    packets++;

    // add sorted numbers to original list
    for(unsigned int j = 0 ; j < ntohl(msg.num_values); j++) {
      msg.values[j] = ntohl(msg.values[j]);
      lists[i].get().push_back(msg.values[j]);
    }

  } while(msg.flag == NONE);
}


/*
 * Send the contents of the lists contained with LISTS to the server on HIOSTNAME
 * listening on PORT in datagrams containing batches of unsigned integers. These
 * will be returned to you MSD Radix sorted and should be retied to the caller
 * via LISTS.
 */
void RadixClient::msd(const char *hostname, const int port, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) exit(-1);

  struct hostent *server = gethostbyname(hostname);
  if (server == NULL) exit(-1);

  struct sockaddr_in remote_addr;
  bzero((char *) &remote_addr, sizeof(remote_addr));
  remote_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&remote_addr.sin_addr.s_addr, server->h_length);
  remote_addr.sin_port = htons(port);

  socklen_t len = sizeof(remote_addr);
  Message msg;

  // send each list to the server for sorting
  for(unsigned int i = 0; i < lists.size(); i++) {

    std::vector<unsigned int> currList = lists[i].get();  // copy of current list
    int listSize = lists[i].get().size();                 // size of current list

    bool resendAll = false; // if packets go missing, need to resend them

    // send list to server for sorting and receive sorted list
    msdHelper(msg, currList, listSize, len, sockfd, remote_addr, lists, i, resendAll);
  }
  close(sockfd);
}
