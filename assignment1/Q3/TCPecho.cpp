/* TCPecho.c - main, TCPecho */

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <errno.h>

using namespace std;
void	TCPecho(const char *host, const char *service);
int	errexit(const char *format, ...);

int	connectsock(const char *host, const char *service,
		const char *transport);

#define	MAX	4096

void debug(char* p) {
	return;
	printf("%s\n", p);
}

/*------------------------------------------------------------------------
 * main - TCP client for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*host = (char*)"localhost";	/* host to use if none supplied	*/
	char	*service = (char*)"echo";	/* default service name		*/

	switch (argc) {
	case 1:
		host = (char*)"localhost";
		break;
	case 3:
		service = argv[2];
		/* FALL THROUGH */
	case 2:
		host = argv[1];
		break;
	default:
		fprintf(stderr, "usage: TCPecho [host [port]]\n");
		exit(1);
	}

	TCPecho(host, service);

	exit(0);
}

bool is_correct(char* s) {
	// counting the number of words
	stringstream ss(s);
	string word;
	int count = 0;
	while(ss >> word) {
		if(count > 0) {
			for(char c: word) {
				if(!(c >= '0' && c <= '9')) return false;
			}
		}
		count++;
	}

	if(count == 2) return true;
	return false;
}

char* substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;
 
    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
 
    // extracts characters between m'th and n'th index from source string
    // and copy them into the destination string
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
 
    // null-terminate the destination string
    *dest = '\0';
 
    // return the destination string
    return dest - len;
}

void createFile(char* filename, char* content) {

	int i = 0;
	while(filename[i] != '.') i++;

	sprintf(filename, "%s1.%s", substr(filename, 0, i), substr(filename, i+1, strlen(filename)));
	
	FILE *fp = fopen(filename, "w");

	if (!fp)
	{
		printf("Error: Unable to create the file\n");
		exit(0);
		return;
	}


	fprintf(fp, "%s", content);

	// Close the file
	fclose(fp);
	printf("Data written to file successfully\n");
}


/*------------------------------------------------------------------------
 * TCPecho - send input to ECHO service on specified host and print reply
 *------------------------------------------------------------------------
 */
void 
TCPecho(const char *host, const char *service)
{
	char buff[MAX];
	char response[MAX];
	int	sockfd;			/* socket descriptor, read count*/
	int n, er;
	struct sockaddr_in serveraddr;
	socklen_t len;

	sockfd = 	connectsock(host, service, "tcp");
	if (sockfd < 0)
	  {
		fprintf(stderr, "usage: TCPecho connectsock failed. \n");
		exit(1);
	  }
	
	len = sizeof(serveraddr);
	getpeername(sockfd, (struct sockaddr*)&serveraddr, &len);

	printf("************** \n");
	printf("Server IP address: %s\n", inet_ntoa(serveraddr.sin_addr));
	printf("Server port      : %d\n", ntohs(serveraddr.sin_port));
	printf("************** \n");


	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		buff[n] = '\0';
		debug(buff);

		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			er = write(sockfd, buff, sizeof(buff));
			if (er < 0)
				errexit("socket write failed: %s\n",
					strerror(errno));
			break;
		}

		if(!(is_correct(buff))) {
			printf("Enter request in correct format!\n");
			continue;
		}
		er = write(sockfd, buff, sizeof(buff));
		if (er < 0)
				errexit("socket write failed: %s\n",
					strerror(errno));

		char* filename = strtok(buff, " ");
		char* numberOfBytes = strtok(NULL, " ");

		debug(filename);
		debug(numberOfBytes);

		er = read(sockfd, response, sizeof(response));
		debug(response);
		if (er < 0)
				errexit("socket read failed: %s\n",
					strerror(errno));

		if(strcmp(response, "404") == 0) {
			printf("Server says that the file does not exist.\n");
		} else {
			createFile(filename, response);
		}
	}
}
