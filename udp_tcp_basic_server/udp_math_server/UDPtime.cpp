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
#include <sstream>
#include <time.h>

using namespace std;

#define	BUFSIZE 64
#define MAXLINE 1024

#define	UNIXEPOCH	2208988800UL	/* UNIX epoch, in UCT secs	*/
#define	MSG		"what time is it?\n"

#include <errno.h>
int	errexit(const char *format, ...);

int
connectsock(const char *host, const char *service, const char *transport );

void debug(char *s) {
	return;
	cout << s << endl;
}

bool is_correct(char* s) {
	// counting the number of words
	stringstream ss(s);
	string word;
	int count = 0;
	while(ss >> word) {
		if(count > 0) {
			for(char c: word) {
				if(!(c >= '0' && c <='9')) return false;
			}
		}
		count++;
	}

	if(count == 3) return true;
	return false;
}

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

	while(1) {

		printf("Enter Command:\t");
		bzero(buffer, sizeof(buffer));
		n = 0;
		while ((buffer[n++] = getchar()) != '\n')
			;

		if(buffer[n] == EOF) {
			printf("Client Requests have ended.");
			printf("Terminating Process");
			exit(0);
		}

		buffer[n] = '\0';

		if(strcmp(buffer, "exit\n") == 0) break;

		if(!is_correct(buffer)) {
			printf("Please enter correct command\n");
			continue;
		}
		
		int k = sendto(sockfd, (char *)buffer, strlen(buffer),
			MSG_CONFIRM, (const struct sockaddr *) &servaddr,
				sizeof(servaddr));

		if(k < 0) {
			perror("Failed Sending message to the Server.");
			exit(EXIT_FAILURE);
		}
			
		bzero(buffer, sizeof(buffer));
		n = 0;
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
					MSG_WAITALL, (struct sockaddr *) &servaddr,
					&len);
		buffer[n] = '\0';

		debug(buffer);

		if(n < 0) {
			perror("Failed to recieve echoed message from server.");
			exit(1);
		}

		std::cout<<"Answer from server:\t"<<buffer << std::endl;

	}

	close(sockfd);
	return 0;
}
