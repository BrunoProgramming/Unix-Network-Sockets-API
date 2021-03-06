#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_MESG_SIZE 4096
char mesg[MAX_MESG_SIZE] = "";

int main(int argc, char *argv[]) {
	int udpSocket = 0, myPort = 0, status = 0, size = 0, clientLength = 0;
	struct sockaddr_in serverName = { 0 }, clientName = { 0 };

	if (2 != argc) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	myPort = atoi(argv[1]);

	udpSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (-1 == udpSocket) {
		perror("socket()");
		exit(1);
	}

	memset(&serverName, 0, sizeof(serverName));
	memset(&clientName, 0, sizeof(clientName));

	serverName.sin_family = AF_INET;
	serverName.sin_addr.s_addr = htonl(INADDR_ANY);
	serverName.sin_port = htons(myPort);

	status = bind(udpSocket, (struct sockaddr *) &serverName,
			sizeof(serverName));
	if (-1 == status) {
		perror("bind()");
		exit(1);
	}

	for (;;) {
		size = recvfrom(udpSocket, mesg,
		MAX_MESG_SIZE, 0, (struct sockaddr *) &clientName, &clientLength);
		if (size == -1) {
			perror("recvfrom()");
			exit(1);
		}

		status = sendto(udpSocket, mesg, size, 0,
				(struct sockaddr *) &clientName, clientLength);
		if (status != size) {
			fprintf(stderr, "sendto(): short write.\n");
			exit(1);
		}
	}

	/* never reached */
	return 0;
}

