// reference : Beej's Guide to Network Programming
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
#define PORT 24539 // TCP port for student id 2178884539
#define PORTA 21539
#define PORTB 22539
#define PORTUDP 23539 // UDP port for student id 2178884539

#define BACKLOG 2

int main(void)
{
	// reference : geeksforgeeks TCP Server-Client implementation in C
	int parent_sockfd, child_sockfd;
	struct sockaddr_in servaddr, clientaddr;
	socklen_t len;

	// socket create and verification 
    parent_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (parent_sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 

    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT); 

    int bd = bind(parent_sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("[+]Bind to Port number %d.\n", PORT);

    listen(parent_sockfd, BACKLOG);
    printf("[+]Listening...\n");

    while(1){
    	child_sockfd = accept(parent_sockfd, (struct sockaddr*)&clientaddr, &len);
    	printf("received\n");
    }
	
}