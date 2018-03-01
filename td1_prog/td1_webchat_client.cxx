#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

void exitErreur(const char * msg) {
	perror(msg);
	exit( EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {

  int sock_client = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sockaddr_serveur;
  sockaddr_serveur.sin_family = AF_INET;
  sockaddr_serveur.sin_port = htons(50013);//server port
  sockaddr_serveur.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(connect(sock_client, (struct sockaddr *) &sockaddr_serveur,sizeof(sockaddr_in)) == -1){
    exitErreur("connect");
  };

  const char * cmsg = "Ceci est un message automatique\n";
  if (write(sock_client, cmsg, strlen(cmsg)) == -1) exitErreur("write");
  char buffer[50];
  string result;
  int lu(0);
  while(1<2){
    if(lu = read(sock_client, buffer, sizeof(buffer))){
      string bufferLu = string(buffer,lu);
      std::cout << bufferLu << endl ;
    }
    string msg;
    getline(cin,msg);
    const char * cmsg = msg.c_str();
    if (write(sock_client, cmsg, strlen(cmsg)) == -1) exitErreur("write");

  }

  //close(sock_client);
  cout << "Client fermé" << endl;
  return 0;
}
