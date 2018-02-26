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
  sockaddr_serveur.sin_port = htons(80);//server port
  sockaddr_serveur.sin_addr.s_addr = inet_addr("10.203.9.220");

  if(connect(sock_client, (struct sockaddr *) &sockaddr_serveur,sizeof(sockaddr_in)) == -1){
    exitErreur("connect");
  };

	const char * query = "GET /index.html\r\n";
  write(sock_client, query, strlen(query));

  char buffer[50];
  string result;
  int nblu(0);
  while(nblu=read(sock_client, buffer,sizeof(buffer))) {
    result += string(buffer,nblu);
  }
  cout << result << endl;

  //close(sock_client);
  return 0;
}
