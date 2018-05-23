/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include "radix.h"

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

    // int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // if (sockfd < 0) exit(-1);
    //
    // struct sockaddr_in server_addr;
    // bzero((char *) &server_addr, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = INADDR_ANY;
    // server_addr.sin_port = htons(port);
    //
    // if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    //   exit(-1);
    //
    // struct sockaddr_in remote_addr;
    // socklen_t len = sizeof(remote_addr);
    //
    // close(sockfd);
}

/*
 * Shutdown the server. Typically this will involve closing the server socket.
 */
void RadixServer::stop() {
    throw "not implemented";
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

  // int n = sendto(sockfd, argv[3], strlen(argv[3]), 0, (struct sockaddr *)&remote_addr, len);
  // if (n < 0) exit(-1);
  //
  // n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)&remote_addr, len);
  // if (n < 0) exit(-1);

  close(sockfd);
}
