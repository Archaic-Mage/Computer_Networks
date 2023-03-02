/* UDPtimed.c - main */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include <errno.h>

using namespace std;

int	passiveUDP(const char *service);
int	errexit(const char *format, ...);

#define	UNIXEPOCH	2208988800UL	/* UNIX epoch, in UCT secs	*/
#define MAXLINE 1024

int
passivesock(const char *service, const char *transport, int qlen);

void debug(char *s) {
	cout << s << endl;
}

void convert_to_lowercase(char *str) {
    int i;
    for (i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

char* computation(char *data) {

	// debug("In computation");

	char *split[3];
	char *ptr;


	char *ret = (char *) malloc(40*sizeof(char));

	int tok = 0;
	ptr = strtok(data, " ");

	while(ptr != NULL) {
		split[tok] = ptr;
		tok++;
		if(tok == 3) break;
		ptr = strtok(NULL, " ");
	}

	if(tok != 3) {
		sprintf(ret, "Wrong");
		return ret;
	}

	int a = atoi(split[1]);
	int b = atoi(split[2]);

	convert_to_lowercase(split[0]);

	if(strcmp("add", split[0]) == 0) {
		sprintf(ret, "%d", a+b);
	}
	else if(strcmp("mul", split[0]) == 0) {
		sprintf(ret, "%d", a*b);
	}
	else if(strcmp("mod", split[0]) == 0) {
		sprintf(ret, "%d", a%b);
	} 
	else if(strcmp("hyp", split[0]) == 0) {
		sprintf(ret, "%d", (int) sqrt(a*a + b*b));
	}
	else {
		sprintf(ret, "Wrong Operator, Try Again!");
	}

	return ret;
}

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

	while(1) {
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				&len);

		if(n < 0) {
			perror("Recieving Failed by the server.");
			exit(EXIT_FAILURE);
		}

		// debug("--message recieved--");

		buffer[n] = '\0';
		printf("Recieved:\t%s\n", buffer);

		const char* out = computation(buffer);

		n = sendto(sockfd, out, strlen(out),
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
				len);

		if(n < 0) {
			perror("Sending Failed by the server.");
			exit(EXIT_FAILURE);
		}
	}
	
	return 0;
}
