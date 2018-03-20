#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/select.h>


#define NUM_PORT 50013
#define BACKLOG 50
#define NB_CLIENTS 100

using namespace std;

void exitErreur(const char * msg) {
	perror(msg);
	exit( EXIT_FAILURE);
}




int main() {


	struct sockaddr_in sockaddr_serveur;
	sockaddr_serveur.sin_family = AF_INET;
	sockaddr_serveur.sin_port = htons(NUM_PORT);
	sockaddr_serveur.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_aton("127.0.0.1", &sockaddr_serveur.sin_addr);

	int yes = 1;
	// TCP
	int sock_serveur_tcp = socket(AF_INET, SOCK_STREAM, 0);
	if (setsockopt(sock_serveur_tcp, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1)
		exitErreur("setsockopt");

		if (bind(sock_serveur_tcp, (struct sockaddr *) &sockaddr_serveur,
				sizeof(sockaddr_in)) == -1)
			exitErreur("bind");

	if (listen(sock_serveur_tcp, BACKLOG) == -1)
		exitErreur("listen");

	// UDP

  int sock_serveur_udp = socket(AF_INET, SOCK_DGRAM, 0);

	if (setsockopt(sock_serveur_udp, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1)
		exitErreur("setsockopt");
	if (bind(sock_serveur_udp, (struct sockaddr *) &sockaddr_serveur,
			sizeof(sockaddr_in)) == -1)
		exitErreur("bind udp");




	sockaddr_in sockaddr_client;
	socklen_t size = sizeof(sockaddr_client);

	char * msg;
	time_t date;

	cout << "Serveur DayTime lancé  sur le port " << NUM_PORT << endl;

	fd_set readfds, writefds,exceptfds;
	int max;

	for (int i = 1; i <= NB_CLIENTS; i++) {

		FD_ZERO(&readfds);
		FD_SET(sock_serveur_udp,&readfds);
		FD_SET(sock_serveur_tcp,&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);

		if(sock_serveur_udp > sock_serveur_tcp){
			max=sock_serveur_udp;
		}else{
			max=sock_serveur_tcp;
		}

		select(max+1,&readfds,NULL,NULL,NULL);

		if(FD_ISSET(sock_serveur_udp,&readfds)){
			std::cout << "Client UDP" << '\n';
			char buf[500];
			recvfrom(sock_serveur_udp, buf, sizeof(buf), 0,
				(struct sockaddr *) &sockaddr_client, &size);
			date = time(NULL);
			msg = ctime(&date);

			if (sendto(sock_serveur_udp, msg, strlen(msg), 0,
					(struct sockaddr *) &sockaddr_client, sizeof(sockaddr_client))
					== -1)
				exitErreur("sendto");
		}
		if(FD_ISSET(sock_serveur_tcp,&readfds)){
				std::cout << "Client TCP" << '\n';
				int sock_client = accept(sock_serveur_tcp, NULL, NULL);
				if (sock_client == -1)
					exitErreur("accept");

				date = time(NULL);
				msg = ctime(&date);

				if (write(sock_client, msg, strlen(msg)) == -1)
					exitErreur("write");

				close(sock_client);
		}


	}

	return 0;
}
