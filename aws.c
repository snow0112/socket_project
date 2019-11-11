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
	int parent_sockfd, child_sockfd, A_sockfd, B_sockfd;
	struct sockaddr_in servaddr, clientaddr, A_servaddr, B_servaddr;
	socklen_t len, A_len, B_len;
    char bufferA[1024];
    char bufferB[1024];

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

        bzero(&A_servaddr, sizeof(A_servaddr)); 
        A_servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        A_servaddr.sin_port = htons(PORTA); 
        A_servaddr.sin_family = AF_INET;
        A_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        connect( A_sockfd, (struct sockaddr *) &A_servaddr, sizeof(A_servaddr) );
        printf("connect to A\n");
        char *message = "Hello Server";
        sendto(A_sockfd, message, 1000, 0, (struct sockaddr*)NULL, sizeof(A_servaddr));
        recvfrom(A_sockfd, bufferA, sizeof(bufferA), 0, (struct sockaddr*)NULL, NULL); 
        puts(bufferA); 
        close(A_sockfd);

        bzero(&B_servaddr, sizeof(B_servaddr)); 
        B_servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        B_servaddr.sin_port = htons(PORTB); 
        B_servaddr.sin_family = AF_INET;
        B_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        connect( B_sockfd, (struct sockaddr *) &B_servaddr, sizeof(B_servaddr) );
        printf("connect to B\n");
        char *message2 = "Hi Server B";
        sendto(B_sockfd, message2, 1000, 0, (struct sockaddr*)NULL, sizeof(B_servaddr));
        recvfrom(B_sockfd, bufferB, sizeof(bufferB), 0, (struct sockaddr*)NULL, NULL); 
        puts(bufferB); 
        close(B_sockfd);


    }
	
}