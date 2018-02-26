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
  sockaddr_serveur.sin_addr.s_addr = inet_addr("139.124.187.164");

  if(connect(sock_client, (struct sockaddr *) &sockaddr_serveur,sizeof(sockaddr_in)) == -1){
    exitErreur("connect");
  };
  
  char buffer[1];
  string result;
  int nblu(0);
  while(nblu=read(sock_client, buffer,sizeof(buffer))) {
    result += string(buffer,nblu);
  }
  cout << result << endl;

  //close(sock_client);
  return 0;
}
