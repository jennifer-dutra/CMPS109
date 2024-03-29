
/*
 * Referenced Distributed lecutre notes:
 * https://classes.soe.ucsc.edu/cmps109/Spring18/SECURE/15.Distributed5.pdf
 * https://classes.soe.ucsc.edu/cmps109/Spring18/SECURE/13.Distributed3.pdf
 *
 */


#include "crack.h"

// crack passwords
void crackPass(Message& msg, uint i) {

  int plainSize = 5;
  char plain[plainSize];
  crack(msg.passwds[i], plain);                       // crack password
  memset(msg.passwds[i], 0, sizeof(msg.passwds[i]));  // clear original array

  // replace hash with plain text
  for(int j = 0; j <= plainSize; j++) {
    if(j == plainSize) {
      msg.passwds[i][j] = '\0';
      continue;
    }
    msg.passwds[i][j] = plain[j];
  }

  std::cout << "decryped: " << msg.passwds[i] << std::endl;

}


// cracker
void CrackClient::cracker() {

  // receive the UDP packet sent from the server
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) exit(-1);

  struct sockaddr_in server_addr;
  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(get_multicast_port());

  if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    exit(-1);

  // multicast receiver
  struct ip_mreq multicastRequest;
  multicastRequest.imr_multiaddr.s_addr = get_multicast_address();
  multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicastRequest, sizeof(multicastRequest)) < 0)
    exit(-1);

  // initialize message
  Message msg;
  msg.num_passwds = 0;
  msg.port = 0;

  // receive the message
  recvfrom(sockfd, (void*)&msg, sizeof(Message), 0, NULL, 0);

  msg.num_passwds = ntohl(msg.num_passwds);

  // PRINT TESTING
  // std::cout << msg.cruzid << std::endl;
  // std::cout << msg.num_passwds << std::endl;
  // std::cout << msg.hostname << std::endl;

  std::thread threads[msg.num_passwds];                 // create array of threads
  unsigned int currThreads = 0;                         // current number of threads running
  unsigned int totalThreads = 0;                        // total threads used
  unsigned int maxCores = 24;

  // sort each bucket
  for(unsigned int i = 0; i < msg.num_passwds; i++) {
    threads[currThreads] = std::thread(crackPass, std::ref(msg), i);
    currThreads++;
    totalThreads++;

    if(currThreads == maxCores || totalThreads ==  msg.num_passwds) {
      for(unsigned int k = 0; k < currThreads; k++) {
        threads[k].join();
      }
      currThreads = 0;
    }
  }

  // crack password
  // crackPass(msg);

  msg.num_passwds = htonl(msg.num_passwds);

  close(sockfd);


  // send back using TCP protocol

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    // std::cout << "SOCK FD FAIL" << '\n';
    exit(-1);
  }

  struct hostent *server = gethostbyname(msg.hostname);
  if (server == NULL) {
    // std::cout << "SERVER IS NULL" << '\n';
    exit(-1);
  }

  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

  serv_addr.sin_port = msg.port;

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    // std::cout << "CONNECT FAIL" << '\n';
    exit(-1);
  }

  // std::cout << "sending: " << msg.passwds[0] << std::endl;

  send(sockfd, (void*)&msg, sizeof(Message), 0);

  // std::cout << "send status: " << n << '\n';

  close(sockfd);

}


int main() {

  CrackClient client;
  client.cracker();

}
