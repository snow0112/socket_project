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


int main(void)
{
	// reference : geeksforgeeks TCP Server-Client implementation in C
	int parent_sockfd, child_sockfd, A_sockfd, B_sockfd;
	struct sockaddr_in servaddr, clientaddr, A_servaddr, B_servaddr;
	socklen_t len, A_len, B_len;
    char buffer[1024]; // for sending result to client
    //char bufferA[1024]; // for saving the result form server A
    char bufferB[1024]; // for saving the result form server B
    char mapID[10];
    int source, filesize;

	// socket create and verification 
    parent_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (parent_sockfd == -1) exit(0); 

    bzero(&servaddr, sizeof(servaddr)); 
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT); 

    int bd = bind(parent_sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    //printf("[+]Bind to Port number %d.\n", PORT);
    listen(parent_sockfd, 3);
    printf("The AWS is up and running.\n"); // only while starting
    while(1){

    	child_sockfd = accept(parent_sockfd, (struct sockaddr*) &clientaddr, &len);
        recv(child_sockfd, mapID, 1024, 0);
        mapID[1] = '\0';
        recv(child_sockfd, &source, sizeof(source), 0);
        recv(child_sockfd, &filesize, sizeof(filesize), 0);
        printf("The AWS has received map ID %s, ",mapID);
        printf("start vertex %d, ",source);
        printf("and file size %d from the client using TCP over port 24539.\n", filesize);

        bzero(&A_servaddr, sizeof(A_servaddr)); 
        A_servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        A_servaddr.sin_port = htons(PORTA); 
        A_servaddr.sin_family = AF_INET;
        A_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        connect( A_sockfd, (struct sockaddr *) &A_servaddr, sizeof(A_servaddr) );

        sendto(A_sockfd, mapID, sizeof(mapID), 0, (struct sockaddr*)NULL, sizeof(A_servaddr));
        sendto(A_sockfd, &source, sizeof(source), 0, (struct sockaddr*)NULL, sizeof(A_servaddr));
        printf("%s\n","The AWS has sent map ID and starting vertex to server A using UDP over port 21539.");

        double propagation;
        double transmission;
        int m; // vertex number
        int paths[10][2];
        recvfrom(A_sockfd, &propagation, sizeof(double), 0, (struct sockaddr*)NULL, NULL);
        recvfrom(A_sockfd, &transmission, sizeof(double), 0, (struct sockaddr*)NULL, NULL);
        recvfrom(A_sockfd, &m, sizeof(int), 0, (struct sockaddr*)NULL, NULL);
        printf("%f\n",propagation );
        printf("%f\n",transmission );
        printf("%d\n",m );
        for(int i = 0; i < m-1; i++){
            recvfrom(A_sockfd, &paths[i][0], sizeof(int), 0, (struct sockaddr*)NULL, NULL);
            recvfrom(A_sockfd, &paths[i][1], sizeof(int), 0, (struct sockaddr*)NULL, NULL);
        }
        //recvfrom(A_sockfd, bufferA, sizeof(bufferA), 0, (struct sockaddr*)NULL, NULL); 
        printf("%s\n", "The AWS has received shortest path from server A:");
        printf("%s\n", "-----------------------------");
        printf("%s\n", "Destination        Min Length");
        printf("%s\n", "-----------------------------");
        //puts(bufferA); 
        for(int i = 0; i < m-1; i++){
            printf("%-19d", paths[i][0] );
            printf("%d\n", paths[i][1] );
        }
        printf("%s\n", "-----------------------------");
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
        printf("%s\n","The AWS has sent path length, propagation speed and transmission speed to server B using UDP over port 22539.");
        recvfrom(B_sockfd, bufferB, sizeof(bufferB), 0, (struct sockaddr*)NULL, NULL); 
        printf("%s\n", "The AWS has received delays from server B:" );
        printf("%s\n", "--------------------------------------------");
        printf("%s\n", "Destination        Tt        Tp        Delay");
        printf("%s\n", "--------------------------------------------");
        puts(bufferB); 
        printf("%s\n", "--------------------------------------------");
        close(B_sockfd);

        strcpy(buffer, "AWS responding");
        send(child_sockfd, buffer, strlen(buffer), 0);
        printf("%s\n", "The AWS has sent calculated delay to client using TCP over port 24539.");
        close(child_sockfd);

    }
	
}