
#include "crack.h"

// crack passwords
void crackPass(Message msg) {
  for(uint i = 0; i < msg.num_passwds; i++) {
    char plain[5];
    crack(msg.passwds[i], plain);                       // crack password
    memset(msg.passwds[i], 0, sizeof(msg.passwds[i]));  // clear original array
    strcpy(msg.passwds[i], plain);                      // add decrypted pass
    std::cout << plain  << std::endl;
  }
}


// cracker
void CrackClient::cracker() {

  int port = get_multicast_port();

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

  msg.num_passwds = 0;
  msg.port = 0;

  recvfrom(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, &len);
  msg.num_passwds = ntohl(msg.num_passwds);

  // PRINT TESTING 
  std::cout << msg.cruzid << std::endl;
  std::cout << msg.num_passwds << std::endl;
  std::cout << msg.hostname << std::endl;

  crackPass(msg);

  msg.num_passwds = htonl(msg.num_passwds);


  // get unicast Port
  // send cracked passwords back in package

  close(sockfd);
}


int main() {

  CrackClient client;
  client.cracker();

}
