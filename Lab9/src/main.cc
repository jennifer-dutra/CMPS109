
#include "crack.h"


// cracker
void CrackClient::crack() {

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

  // struct hostent *server = gethostbyname("localhost");
  // if (server == NULL) exit(-1);
  //
  // struct sockaddr_in remote_addr;
  // bzero((char *) &remote_addr, sizeof(remote_addr));
  // remote_addr.sin_family = AF_INET;
  // bcopy((char *)server->h_addr, (char *)&remote_addr.sin_addr.s_addr, server->h_length);
  // remote_addr.sin_port = htons(port);

  // socklen_t len = sizeof(remote_addr);

  Message msg;

  msg.num_passwds = 0;
  msg.port = 0;

  recvfrom(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, &len);
  msg.num_passwds = ntohl(msg.num_passwds);
  msg.port = ntohl(msg.port);

  std::cout << msg.cruzid << std::endl;

  close(sockfd);
}



int main() {
  // char plain[8];
  // crack("xxo0q4QVK0mOg", plain);
  // printf("Plain: %s\n", plain);

  CrackClient client;
  client.crack();

}
