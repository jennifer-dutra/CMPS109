
#include "crack.h"


void CrackServer::start() {
  // int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  // if (sockfd < 0) exit(-1);
  //
  // struct sockaddr_in server_addr;
  // bzero((char *) &server_addr, sizeof(server_addr));
  // server_addr.sin_family = AF_INET;
  // server_addr.sin_addr.s_addr = INADDR_ANY;
  // server_addr.sin_port = htons(get_port());
  //
  // if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
  //   exit(-1);
  //
  // struct sockaddr_in remote_addr;
  // socklen_t len = sizeof(remote_addr);
  //
  // Message msg;

  // msg.cruzid = "jrdutra";
  // msg.passwds[0][HASH_LENGTH + 1] = "xxo0q4QVK0mOg";
  // msg.num_passwd = 1;
  // msg.hostname;
  // msg.port = port;

  // std::string cruzid = "jrdutra";
  // std::password pass = "xxo0q4QVK0mOg";

  // for(int i = 0; i < ) {

  // }

}


void sendUDP(Message msg, std::string cruzid, std::string hash, int num_passwds, std::string hostname, int port,
  socklen_t len, int sockfd, struct sockaddr_in remote_addr) {

  // set cruzid
  for(uint i = 0; i < cruzid.length(); i++) {
    msg.cruzid[i] = cruzid.at(i);
  }

  // set hash
  for(uint i = 0; i < hash.length(); i++) {
    msg.passwds[0][i] = hash.at(i);
  }

  // set host name
  for(uint i = 0; i < hostname.length(); i++) {
    msg.hostname[i] = hostname.at(i);
  }


  msg.num_passwds = htonl(num_passwds);
  msg.port = htonl(port);

  sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *)&remote_addr, len);

}

void CrackClient::crack() {

  int port = get_multicast_port();

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) exit(-1);

  Message msg;

  // set hostname
  std::string hostname = "localhost";
  for(uint i = 0; i < hostname.length(); i++) {
    msg.hostname[i] = hostname.at(i);
  }

  struct hostent *server = gethostbyname(msg.hostname);
  if (server == NULL) exit(-1);

  struct sockaddr_in remote_addr;
  bzero((char *) &remote_addr, sizeof(remote_addr));
  remote_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&remote_addr.sin_addr.s_addr, server->h_length);
  remote_addr.sin_port = htons(port);

  socklen_t len = sizeof(remote_addr);

  int num_passwds = 1;
  std::string cruzid = "jrdutra";
  std::string hash = "xxo0q4QVK0mOg";

  sendUDP(msg, cruzid, hash, num_passwds, hostname, port, len, sockfd, remote_addr);

  close(sockfd);
}



int main() {
  char plain[8];
  crack("xxo0q4QVK0mOg", plain);
  printf("Plain: %s\n", plain);

  CrackClient client;
  client.crack();
}
