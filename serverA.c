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

typedef struct tagmap {
    char mapID[10];
    double propogation;
    double transmission;
    int vertex_number;
    int edge_number;
    int vertrx_sequence[10];
    int adja[10][10];
} Map;

typedef struct Node{
    Map* m;
    struct Node *next;
} Node;

void initNode(Node* node){
  Map* space;
  space = malloc( sizeof(Map) );
  space->mapID[0] = '\0';
  space->propogation = 0;
  space->transmission = 0;
  space->vertex_number = 0;
  space->edge_number = -3;
  for(int i = 0; i < 10; i++){
    space->vertrx_sequence[i] = -1;
  }
  for(int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      space->adja[i][j] = 0;
    }
  }
  node->m = space;
  //node->next = NULL;
}

int main(){

  printf("%s\n","The Server A is up and running using UDP on port 21539" );

  // construct map
  int mapcount = 0;
  Node head;
  initNode(&head);
  //printf("%d\n", *(&head.m->vertex_number));
  //memset(&head, 0, sizeof(Node));
  Node iter = head;

  FILE* fp;
  char filebuff[1024];
  fp = fopen("map.txt", "r");
  while (fgets(filebuff, sizeof(filebuff), fp) != NULL) {
    if (filebuff[0] - '0' < 0 || filebuff[0] - '0' > 8) {
      mapcount++;
      Node next;
      initNode(&next);
      *&iter.next = &next;
      iter = next;
      strcpy(*(&iter.m->mapID), filebuff);
    }
    else if ( (*(&iter.m->edge_number)) == -2 )
      (*(&iter.m->propogation)) = atof(filebuff);
    else if ( (*(&iter.m->edge_number)) == -1 )
      (*(&iter.m->transmission)) = atof(filebuff);
    else{
      //puts(filebuff);
      int v1, v2, w;
      char *start_ptr = filebuff;
      char *tab_ptr = strchr(start_ptr,' ');
      if (tab_ptr != NULL) *tab_ptr++ = '\0';
      v1 = atoi(start_ptr);
      //printf("[+] v1, %d\n", v1);
      start_ptr = tab_ptr;
      tab_ptr = strchr(start_ptr,' ');
      if (tab_ptr != NULL) *tab_ptr++ = '\0';
      v2 = atoi(start_ptr);
      //printf("[+] v2, %d\n", v2);
      start_ptr = tab_ptr;
      w = atoi(start_ptr);
      //printf("[+] w, %d\n", w);

      // find index of v1 and v2
      int idx1 = 9;
      int idx2 = 9;
      for(int i = 0; i < 10; i++){
        if ( (*(&iter.m->vertrx_sequence[i])) == -1 || (*(&iter.m->vertrx_sequence[i])) == v1 ){
          if ( (*(&iter.m->vertrx_sequence[i])) == -1 ) {
            //printf("add vertex, %d\n",v1 );
            (*(&iter.m->vertex_number))++;
          }
          (*(&iter.m->vertrx_sequence[i])) = v1;
          idx1 = i;
          break;
        }
      }
      for(int i = 0; i < 10; i++){
        if ( (*(&iter.m->vertrx_sequence[i])) == -1 || (*(&iter.m->vertrx_sequence[i])) == v2 ){
          if ( (*(&iter.m->vertrx_sequence[i])) == -1 ) {
            //printf("add vertex, %d\n",v2 );
            (*(&iter.m->vertex_number))++;
          }
          (*(&iter.m->vertrx_sequence[i])) = v2;
          idx2 = i;
          break;
        }
      }
      // build graph
      (*(&iter.m->adja[idx1][idx2])) = w;
      (*(&iter.m->adja[idx2][idx1])) = w;
    }
    (*(&iter.m->edge_number))++;
    //puts(filebuff);
    //printf("[+] vertes number, %d\n", *(&iter.m->vertex_number));
    //printf("[+] propogation, %.2f\n", *(&iter.m->propogation));
    //printf("[+] transmission, %.2f\n", *(&iter.m->transmission));
  }
  fclose(fp);

  printf("The Server A has constructed a list of %d maps:\n", mapcount);
  printf("%s\n", "-------------------------------------------");
  printf("%s\n", "Map ID  Num Vertices  Num Edges");
  printf("%s\n", "-------------------------------------------");
  char* pmapbuff[45];
  pmapbuff[44] = '\0';
  //iter = **(&head.next);
  for (int i = 0; i < mapcount; i++){
    //iter = **(&iter.next);
    printf("%s\n", *(&iter.m->mapID));
    for (int j = 0; j < 44; j++) pmapbuff[j] = " ";
  }
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
    break;
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