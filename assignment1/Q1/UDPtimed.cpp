/* UDPtimed.c - main */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <errno.h>

using namespace std;

int	passiveUDP(const char *service);
int	errexit(const char *format, ...);

#define	UNIXEPOCH	2208988800UL	/* UNIX epoch, in UCT secs	*/
#define MAXLINE 1024

int
passivesock(const char *service, const char *transport, int qlen);

/*------------------------------------------------------------------------
 * main - Iterative UDP server for TIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	struct sockaddr_in servaddr, cliaddr;	/* the from address of a client	*/
	char	*service = (char *)"echo";	/* service name or port number	*/
	char	buf[1];			/* "input" buffer; any size > 0	*/
	int	sockfd;			/* server socket		*/
	char buffer[MAXLINE];
	unsigned int	len;		/* from-address length		*/

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: UDPtimed [port]\n");
	}

	sockfd = passivesock(service, "udp", 0);
	/* Last parameter is Queue length and not applicable for UDP sockets*/

	int n;
	len = sizeof(servaddr);

	cout << "****************" << endl;
	cout << "Server is Up and Running: " << endl;
	printf("Server IP address: %s\n", inet_ntoa(servaddr.sin_addr));
	printf("Server port      : %d\n", ntohs(servaddr.sin_port));
	cout << "****************" << endl;
	int t = 1;
	while(t--) {
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				&len);

		if(n < 0) {
			perror("Recieving Failed by the server.");
			exit(EXIT_FAILURE);
		}

		buffer[n] = '\0';
		printf("****************\n");
		printf("Recieved message from client.\n");
		printf("Client IP address: %s\n", inet_ntoa(cliaddr.sin_addr));
		printf("Client port 	 : %d\n", ntohs(cliaddr.sin_port));
		printf("Client message 	 : %s", buffer);
		printf("****************\n");

		// n = sendto(sockfd, (const char *)buffer, strlen(buffer),
		// 	MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
		// 		len);

		// if(n < 0) {
		// 	perror("Sending Failed by the server.");
		// 	exit(EXIT_FAILURE);
		// }
		
		// std::cout<<"Message echoed."<<std::endl;
		// cout << "****************" << endl; 
	}
	
	return 0;
}
