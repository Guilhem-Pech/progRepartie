#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BACKLOG 50
#define NB_CLIENTS 100
#define TAILLE_BUFFER 50

using namespace std;

void exitErreur(const char * msg) {
	perror(msg);
	exit( EXIT_FAILURE);

}

int main(int argc, char * argv[]) {
	int port = atoi("5040");

	int sock_serveur = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM= UDP ~ SOCK_STREAM en TCP

	struct sockaddr_in sockaddr_serveur;

	sockaddr_serveur.sin_family = AF_INET;
	sockaddr_serveur.sin_port = htons(port);
	sockaddr_serveur.sin_addr.s_addr = INADDR_ANY; // Utilise toutes ses adresses et le port est passée en paramètre

	int yes = 1;
	if (setsockopt(sock_serveur, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1)
		exitErreur("setsockopt");

	if (bind(sock_serveur, (struct sockaddr *) &sockaddr_serveur,
			sizeof(sockaddr_in)) == -1)
		exitErreur("bind");

	sockaddr_in sockaddr_client;
	socklen_t size = sizeof(sockaddr_client);

	char buf[TAILLE_BUFFER];
	int nbOctetsRecus;
	cout << "Serveur Echo UDP lancé  sur le port " << port << endl;

	for (int i = 1; i <= NB_CLIENTS; i++) {

		if ( (nbOctetsRecus = recvfrom(sock_serveur, buf, sizeof(buf), 0, // Il se bloque ici
				(struct sockaddr *) &sockaddr_client, &size)) ==
				-1)exitErreur("recvfrom");



	cout << string(buf,nbOctetsRecus) << endl;

		if (sendto(sock_serveur, buf, nbOctetsRecus, 0,
				(struct sockaddr *) &sockaddr_client, sizeof(sockaddr_client))
				== -1)
			exitErreur("sendto");

	}
	close(sock_serveur);
	return EXIT_SUCCESS;
}
