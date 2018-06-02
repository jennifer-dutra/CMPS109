
#include "crack.h"

// crack passwords
void crackPass(Message msg) {

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


  // crack password 
  for(uint i = 0; i < msg.num_passwds; i++) {
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

  msg.num_passwds = htonl(msg.num_passwds);


  // get unicast Port
  // send cracked passwords back in package

  close(sockfd);

  // send back using TCP protocol

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) exit(-1);

  struct hostent *server = gethostbyname("localhost");
  if (server == NULL) exit(-1);

  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

  serv_addr.sin_port = htons(get_unicast_port());

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) exit(-1);

  std::cout << "sending: " << msg.passwds[0] << std::endl;

  int n = send(sockfd, (void*)&msg, sizeof(Message), 0);

  std::cout << "send status: " << n << '\n';

  close(sockfd);

}


int main() {

  CrackClient client;
  client.cracker();

}
