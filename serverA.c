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
#define PORT 21539

int main(){

  printf("%s\n","The Server A is up and running using UDP on port 21539" );

  // construct map
  int mapcount;
  printf("The Server A has constructed a list of %d maps:\n", mapcount);
  printf("%s\n", "-------------------------------------------");
  printf("%s\n", "Map ID  Num Vertices  Num Edges");
  printf("%s\n", "-------------------------------------------");
  printf("%s\n", "-------------------------------------------");

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

  char buffer[1024]; // for sending back to aws
  char mapID[1024];
  int source;  
  while(1){
    int n = recvfrom(sockfd, mapID, 1024, 0, (struct sockaddr*) &clientaddr, &len);
    mapID[n] = '\0';
    recvfrom(sockfd, &source, sizeof(source), 0, (struct sockaddr*) &clientaddr, &len);
    printf("The Server A has received input for finding shortest paths: starting vertex %d ", source);
    printf("of map %s.\n", mapID);

    // dijkstra

    printf("%s\n", "The Server A has identified the following shortest paths:");
    printf("%s\n", "-----------------------------");
    printf("%s\n", "Destination  Min Length");
    printf("%s\n", "-----------------------------");
    printf("%s\n", "-----------------------------");

    char *message = "Here is A responding";
    sendto(sockfd, message, 1024, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    printf("%s\n", "The Server A has sent shortest paths to AWS.");

  }

}