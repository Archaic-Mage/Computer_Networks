/* TCPechod.c - main, TCPechod */

#define	_USE_BSD
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

#define	QLEN		  32	/* maximum connection queue length	*/
#define	MAX		4096

#include <errno.h>

using namespace std;

void	reaper(int);
int	TCPechod(int fd);
int	errexit(const char *format, ...);
int	passivesock(const char *service, const char *transport, int qlen);

void debug(char* p) {
	return;
	printf("%s\n", p);
}

/*------------------------------------------------------------------------
 * main - Concurrent TCP server for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*service = (char*)"echo";	/* service name or port number	*/
	struct	sockaddr_in fsin;	/* the address of a client	*/
	unsigned int	alen;		/* length of client's address	*/
	int	msock;			/* master server socket		*/
	int	ssock;			/* slave server socket		*/

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: TCPechod [port]\n");
	}

	msock = passivesock(service, "tcp", QLEN);

	(void) signal(SIGCHLD, reaper);

	while (1) {
	  alen = sizeof(fsin);
	  ssock = accept(msock, (struct sockaddr *)&fsin, &alen);

	  if (ssock < 0) {
			if (errno == EINTR)
				continue;
			errexit("accept: %s\n", strerror(errno));
		}

	  switch (fork()) {

	  case 0:		/* child */
	    (void) close(msock);
	    exit(TCPechod(ssock));

	  default:	/* parent */
	    (void) close(ssock);
	    break;

		case -1:
			errexit("fork: %s\n", strerror(errno));
		}
	}
}

char* readLastN(char *filename, char* numberOfBytes) {
  
  // Open the file for reading
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    printf("Error: Unable to open the file\n");
	return (char*) "404";
  }

  // Get the number of bytes to read from last
  int n = atoi(numberOfBytes);

  // Seek to the end of the file
  if (fseek(fp, -n, SEEK_END) != 0) {
    printf("Error: Unable to seek to the specified position\n");
    fclose(fp);
    return NULL;
  }

  // Allocate memory for the data
  char *data = (char *)malloc(n * sizeof(char));
  if (!data) {
    printf("Error: Unable to allocate memory\n");
    fclose(fp);
	return (char*)"Error: Unable to allocate memory";
  }

  // Read the data
  int bytes_read = fread(data, sizeof(char), n, fp);
  if (bytes_read != n) {
    printf("Error: Unable to read the specified number of bytes\n");
	return (char*) "Incorrect amount of bytes asked.";
    fclose(fp);
    free(data);
  }

  return data;
}

/*------------------------------------------------------------------------
 * TCPechod - echo data until end of file
 *------------------------------------------------------------------------
 */
int
TCPechod(int connfd)
{
	char	buff[BUFSIZ];
	int	n;
	struct sockaddr_in clientaddr;
	socklen_t len;

	len = sizeof(clientaddr);
	getpeername(connfd, (struct sockaddr*)&clientaddr, &len);

	printf("************** \n");
	printf("Client IP address: %s\n", inet_ntoa(clientaddr.sin_addr));
	printf("Client port      : %d\n", ntohs(clientaddr.sin_port));
	printf("************** \n");
	

	for (;;) {
		bzero(buff, BUFSIZ);

		// read the message from client and copy it in buffer
		int n = read(connfd, buff, sizeof(buff));
		if (n < 0)
				errexit("socket read failed: %s\n",
					strerror(errno));
		// print buffer which contains the client contents
		printf("From client: %s\n", buff);

		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Server's child Exit...\n");
			break;
		}

		char* filename = strtok(buff, " ");
		char* numberOfBytes = strtok(NULL, " ");
		char* out = readLastN(filename, numberOfBytes);

		debug(out);
		
		// and send that buffer to client
		write(connfd, out, strlen(out));

		if(strcmp(out, "404") == 0) continue;
		free(out);
	}
	return 0;
}


/*------------------------------------------------------------------------
 * reaper - clean up zombie children
 *------------------------------------------------------------------------
 */
void
reaper(int sig)
{
	int	status;

	while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
		/* empty */;
}
