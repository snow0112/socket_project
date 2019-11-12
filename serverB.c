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

  printf("The Server B is up and running using UDP on port 22539\n");
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
  while(1){
  	recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &clientaddr, &len);
  	buffer[1024] = '\0';
  	printf("The Server B has received data for calculation:\n");
    printf("* Propagation speed: %.2f km/s; \n", 1.1);
    printf("* Transmission speed %.2f Bytes/s; \n", 2.2 );
    printf("* Path length for destination %d: %d;\n",0,1);
  	puts(buffer);
    // calculate delay

    printf("The Server B has finished the calculation of the delays:\n");
    printf("------------------------\n");
    printf("Destination        Delay\n");
    printf("------------------------\n");
    printf("------------------------\n");

    char *message = "Here is B responding";
    sendto(sockfd, message, 1024, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    printf("The Server B has finished sending the output to AWS\n");

  }

}


