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

int main(int argc, char* argv[])
{
    printf("The client is up and running.\n");

	// reference : geeksforgeeks TCP Server-Client implementation in C
	int sockfd;
	struct sockaddr_in servaddr;

	// socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) exit(0); 

    bzero(&servaddr, sizeof(servaddr)); 
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT); 

    int cn = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (cn == -1) exit(0); 
    send(sockfd, argv[1], strlen(argv[1]), 0);
    send(sockfd, argv[2], strlen(argv[2]), 0);
    send(sockfd, argv[3], strlen(argv[3]), 0);

    printf("The client has sent query to AWS using TCP over port 24539; ");
    printf("start vertex %s; ",argv[2]);
    printf("map %s; ",argv[1]);
    printf("file size %s.\n", argv[3]);

    char buffer[1024];
    recv(sockfd, buffer, 1024, 0);
    printf("The client has received results from AWS:\n");
    printf("%s\n","----------------------------------------------");
    printf("%s\n","Destination    Min Length    Tt    Tp    Delay");
    printf("%s\n","----------------------------------------------");
    puts(buffer);
    printf("%s\n","----------------------------------------------");
    close(sockfd); 

	
}