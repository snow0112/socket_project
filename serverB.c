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

  //char buffer[1024];
  while(1){
  	//recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &clientaddr, &len);
  	//buffer[1024] = '\0';

    char filesize[1024];
    double propagation;
    double transmission;
    int m; // vertex number
    int paths[10][2];
    recvfrom(sockfd, &filesize, sizeof(filesize), 0, (struct sockaddr*) &clientaddr, &len);
    recvfrom(sockfd, &propagation, sizeof(double), 0, (struct sockaddr*) &clientaddr, &len);
    recvfrom(sockfd, &transmission, sizeof(double), 0, (struct sockaddr*) &clientaddr, &len);
    recvfrom(sockfd, &m, sizeof(int), 0, (struct sockaddr*) &clientaddr, &len);
    for(int i = 0; i < m-1; i++){
      recvfrom(sockfd, &paths[i][0], sizeof(int), 0, (struct sockaddr*) &clientaddr, &len);
      recvfrom(sockfd, &paths[i][1], sizeof(int), 0, (struct sockaddr*) &clientaddr, &len);
    }
  	printf("The Server B has received data for calculation:\n");
    printf("* Propagation speed: %.2f km/s; \n", propagation);
    printf("* Transmission speed %.2f Bytes/s; \n", transmission );
    for(int i = 0; i < m-1; i++){
      printf("* Path length for destination %d: %d;\n",paths[i][0],paths[i][1]);
    }
    //printf("* Path length for destination %d: %d;\n",0,1);
  	//puts(buffer);
    // calculate delay
    long size = atol(filesize);
    printf("filesize: %s\n",filesize );
    printf("filesize: %ld\n",size );
    double Tt = ((double)0.0/transmission);
    double delays[10][2];
    for(int i = 0; i < m-1; i++) {
      delays[i][0] = (double)paths[i][1]/propagation;
      delays[i][1] = delays[i][0] + Tt;
    }
    printf("Tt : %.2f\n", Tt);
    //for(int i = 0; i < m-1; i++){
    //  printf("%-9d", paths[i][0]);
    //  printf("%f\n",delays[i][0]);
    //}

    printf("The Server B has finished the calculation of the delays:\n");
    printf("--------------------------\n");
    printf("Destination        Delay\n");
    printf("--------------------------\n");
    for(int i = 0; i < m-1; i++){
      printf("%-19d", paths[i][0]);
      printf("%.2f\n",delays[i][1]);
    }
    printf("--------------------------\n");

    sendto(sockfd, &Tt, sizeof(double), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    for(int i = 0; i < m-1; i++){
      sendto(sockfd, &delays[i][0], sizeof(double), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
      sendto(sockfd, &delays[i][1], sizeof(double), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    }

    //char *message = "Here is B responding";
    //sendto(sockfd, message, 1024, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    printf("The Server B has finished sending the output to AWS\n");

  }

}


