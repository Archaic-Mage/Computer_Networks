/* UDPtime.c - main */

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <time.h>

#define	BUFSIZE 64
#define MAXLINE 1024

#define	UNIXEPOCH	2208988800UL	/* UNIX epoch, in UCT secs	*/
#define	MSG		"what time is it?\n"

#include <errno.h>
int	errexit(const char *format, ...);

int
connectsock(const char *host, const char *service, const char *transport );



/*------------------------------------------------------------------------
 * main - UDP client for TIME service that prints the resulting time
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*host = (char *)"localhost";	/* host to use if none supplied	*/
	char	*service = (char *)"echo";	/* default service name		*/
	char buffer[MAXLINE];
	int	sockfd, n;			/* socket descriptor, read count*/
	struct sockaddr_in servaddr;
	socklen_t len;
 
	switch (argc) {
	case 1:
		host = (char *)"localhost";
		break;
	case 3:
		service = argv[2];
		/* FALL THROUGH */
	case 2:
		host = argv[1];
		break;
	default:
		fprintf(stderr, "usage: UDPtime [host [port]]\n");
		exit(1);
	}

	sockfd = connectsock(host, service, "udp");

	len = sizeof(servaddr);
	getpeername(sockfd, (struct sockaddr*)&servaddr, &len);

	printf("****************\n");
	printf("Sending message to server: \n");
	printf("Server IP address: %s\n", inet_ntoa(servaddr.sin_addr));
	printf("Server port 	 : %d\n", ntohs(servaddr.sin_port));
	printf("****************\n");

	while(1) {

		printf("Write your message 	: ");
		bzero(buffer, sizeof(buffer));
		n = 0;
		while ((buffer[n++] = getchar()) != '\n')
			;

		if(buffer[n] == EOF) {
			printf("Client Requests have ended.");
			printf("Terminating Process");
			exit(0);
		}
		
		int k = sendto(sockfd, (char *)buffer, strlen(buffer),
			MSG_CONFIRM, (const struct sockaddr *) &servaddr,
				sizeof(servaddr));

		if(k < 0) {
			perror("Failed Sending message to the Server.");
			exit(EXIT_FAILURE);
		}

		printf("****************\n");
		std::cout<<"Message sent."<<std::endl;
		printf("****************\n");
			
		bzero(buffer, sizeof(buffer));
		n = 0;
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
					MSG_WAITALL, (struct sockaddr *) &servaddr,
					&len);
		buffer[n] = '\0';

		if(n < 0) {
			perror("Failed to recieve echoed message from server.");
			exit(1);
		}

		std::cout<<"Server message 	: "<<buffer;
		printf("****************\n");

	}

	close(sockfd);
	return 0;
}
