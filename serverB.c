#include <stdio.h>
#include <stdlib.h>
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
#define PORT 22539

int main(){
	int sockfd;
	struct sockaddr_in servaddr, clientaddr;
	socklen_t len;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
  len = sizeof(clientaddr);

  char buffer[1024];
  recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &clientaddr, &len);
  buffer[1023] = '\0';
  printf("[+]Data Received: %s \n", buffer);

}