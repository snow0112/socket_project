// reference : Beej's Guide to Network Programming
#include <studio.h>
#include <studlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define PORT "24539" // student id 217 888 4539
#define BACKLOG 20

void sigchld_handler(int s)
{
	// reference : Beej's Guide to Network Programming
	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

int main(void)
{
	int sockfd, newsockfd; // parent = sockfd, children = newsockfd
	struct addrinfo hints, *serverinfo, *p;
	
}