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
    //char filesize[1024];
	struct sockaddr_in servaddr;
    source = atoi(argv[2]);
    //filesize = atol(argv[3]);
    //printf("%d\n", filesize);
    char filesize[1024];
    strcpy(filesize, argv[3]);

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
    send(sockfd, argv[1], strlen(argv[1]), 0);
    send(sockfd, &source, sizeof(int), 0);
    send(sockfd, filesize, sizeof(filesize), 0);

    printf("The client has sent query to AWS using TCP over port 24539; ");
    printf("start vertex %s; ",argv[2]);
    printf("map %s; ",argv[1]);
    printf("file size %s.\n", argv[3]);

    char buffer[1024];
    //recv(sockfd, buffer, 1024, 0);

    int m = 0;
    double Tt = 0.0;
    recv(sockfd, &m, sizeof(int), 0);
    //printf("get m:%d, Length %d\n", m, r); 
    recv(sockfd, &Tt, sizeof(double), 0);
    //printf("%f, %d\n",Tt, r );

    /*

    int paths[10][2];
    double delays[10][2];
    for(int i = 0; i < m-1; i++){
        recv(sockfd, &paths[i][0], sizeof(int), 0);
        recv(sockfd, &paths[i][1], sizeof(int), 0);
        recv(sockfd, &delays[i][0], sizeof(double), 0);
        recv(sockfd, &delays[i][1], sizeof(double), 0);
        //printf("%d,%d,%.2f,%.2f\n",paths[i][0],paths[i][1],delays[i][0],delays[i][1]);
    }*/

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
        printf("%-8.2f",Tt );
        printf("%-8.2f",Tp );
        printf("%.2f\n",delay );
    }
    //puts(buffer);
    printf("%s\n","----------------------------------------------------");
    close(sockfd); 

	
}