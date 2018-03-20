#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_PORT 50013
#define BACKLOG 50
#define NB_CLIENTS 100

using namespace std;

void exitErreur(const char * msg) {
	perror(msg);
	exit( EXIT_FAILURE);

}

int main(int argc, char * argv[]) {
	int port;

	int sock_client = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in sockaddr_serveur;

	sockaddr_serveur.sin_family = AF_INET;
	sockaddr_serveur.sin_port = htons(atoi("5040"));
	inet_aton("127.0.0.1", &sockaddr_serveur.sin_addr);

	socklen_t size = sizeof(struct sockaddr_in);


  string message="Baptiste aime les grosses bites";
  getline(cin,message);

	char buf[message.size()];

	// envoyer n'importe quoi au serveur
	// c'est juste pour lui donner l'adresse de client



  const char* messagec = message.c_str();
	if (sendto(sock_client, messagec, message.size() , 0, (struct sockaddr *) &sockaddr_serveur, sizeof(sockaddr_serveur))== -1)
		exitErreur("sendto");


	// si le message est trop long, il sera tronqué
	int n = recvfrom(sock_client, buf, sizeof(message), 0,
				(struct sockaddr *) &sockaddr_serveur, &size);

	if (n == -1)
		exitErreur("recvfrom");

	string received(buf,n);

	cout << received ;
	close(sock_client);
	return 0;
}
