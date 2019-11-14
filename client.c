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
	int sockfd, source;
	struct sockaddr_in servaddr;
    source = atoi(argv[2]);
    char filesize[1024];
    strcpy(filesize, argv[3]);
	char mapID[10];
	strcpy(mapID, argv[1]);
	mapID[1] = '\0';


	// socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) exit(0); 

    bzero(&servaddr, sizeof(servaddr)); 
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT); 

    int cn = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (cn == -1) {
        printf("connection fail\n");
        exit(0); 
    }

    // send request to aws
    send(sockfd, &mapID[0], sizeof(char), 0);
    int test1 = send(sockfd, &source, sizeof(int), 0);
	printf("%d\n",test1);
    send(sockfd, filesize, sizeof(filesize), 0);

    printf("The client has sent query to AWS using TCP over port 24539; ");
    printf("start vertex %s; ",argv[2]);
    printf("map %s; ",argv[1]);
    printf("file size %s.\n", argv[3]);

    // receive result form aws and print out
    int m = 0;
    double Tt = 0.0;
    recv(sockfd, &m, sizeof(int), 0);
    recv(sockfd, &Tt, sizeof(double), 0);
    int destination, pathlen;
    double Tp, delay;
    printf("The client has received results from AWS:\n");
    printf("%s\n","----------------------------------------------------");
    printf("%s\n","Destination    Min Length    Tt      Tp      Delay");
    printf("%s\n","----------------------------------------------------");
    for(int i = 0; i < m-1; i++){
        recv(sockfd, &destination, sizeof(int), 0);
        recv(sockfd, &pathlen, sizeof(int), 0);
        recv(sockfd, &Tp, sizeof(double), 0);
        recv(sockfd, &delay, sizeof(double), 0);
        printf("%-15d",destination);
        printf("%-14d",pathlen );
        printf("%-7.2f ",Tt );
        printf("%-7.2f ",Tp );
        printf("%.2f\n",delay );
    }
    printf("%s\n","----------------------------------------------------");
    close(sockfd); 
}
