/* UDPtimed.c - main */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

//extern int	errno;

int	passiveUDP(const char *service);
int	errexit(const char *format, ...);

#define	UNIXEPOCH	2208988800UL	/* UNIX epoch, in UCT secs	*/

int
passivesock(const char *service, const char *transport, int qlen);

/*------------------------------------------------------------------------
 * main - Iterative UDP server for TIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	struct sockaddr_in fsin;	/* the from address of a client	*/
	char	*service = "time";	/* service name or port number	*/
	char	buf[1];			/* "input" buffer; any size > 0	*/
	int	sock;			/* server socket		*/
	time_t	now;			/* current time			*/
	unsigned int	alen;		/* from-address length		*/

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: UDPtimed [port]\n");
	}

	sock = passivesock(service, "udp", 0);
	/* Last parameter is Queue length and not applicable for UDP sockets*/

	while (1) {
	  int count;
	  alen = sizeof(fsin);


	  count = recvfrom(sock, buf, sizeof(buf), 0,
			   (struct sockaddr *)&fsin, &alen);

	  printf("************** \n");
	  printf("Client IP address: %s\n", inet_ntoa(fsin.sin_addr));
	  printf("Client port      : %d\n", ntohs(fsin.sin_port));
	  
	  if (count < 0)
	    errexit("recvfrom: %s\n", strerror(errno));

	  now = 30012023;
	  now = htonl((unsigned long)(now));

	  printf("Sending time information...\n");
	  (void) sendto(sock, (char *)&now, sizeof(now), 0,
			(struct sockaddr *)&fsin, sizeof(fsin));

	  printf("************** \n");
	}
}
