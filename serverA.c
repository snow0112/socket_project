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
    //struct Node *next;
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

  // read file

  int mapcount = 0;
  Node maps[100];
  initNode(&maps[0]);
  Node iter;
  FILE* fp;
  char filebuff[1024];
  fp = fopen("map.txt", "r");
  while (fgets(filebuff, sizeof(filebuff), fp) != NULL) {
    if (filebuff[0] - '0' < 0 || filebuff[0] - '0' > 9) {
      mapcount++;
      initNode(&maps[mapcount-1]);
      iter = maps[mapcount-1];
      filebuff[1] = '\0';
      strcpy(*(&iter.m->mapID), filebuff);
    }
    else if ( (*(&iter.m->edge_number)) == -2 )
      (*(&iter.m->propogation)) = atof(filebuff);
    else if ( (*(&iter.m->edge_number)) == -1 )
      (*(&iter.m->transmission)) = atof(filebuff);
    else{
      
      int v1, v2, w;
      char *start_ptr = filebuff;
      char *tab_ptr = strchr(start_ptr,' ');
      if (tab_ptr != NULL) *tab_ptr++ = '\0';
      v1 = atoi(start_ptr);
      
      start_ptr = tab_ptr;
      tab_ptr = strchr(start_ptr,' ');
      if (tab_ptr != NULL) *tab_ptr++ = '\0';
      v2 = atoi(start_ptr);

      start_ptr = tab_ptr;
      w = atoi(start_ptr);

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
  }
  fclose(fp);

  printf("The Server A has constructed a list of %d maps:\n", mapcount);
  printf("%s\n", "-------------------------------------------");
  printf("%s\n", "Map ID  Num Vertices  Num Edges");
  printf("%s\n", "-------------------------------------------");
  for (int i = 0; i < mapcount; i++){
    iter = maps[i];
    printf("%-8s", *(&iter.m->mapID));
    printf("%-14d", *(&iter.m->vertex_number));
    printf("%d\n", *(&iter.m->edge_number));
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
  char mapID[10];
  int source;  
  while(1){
    
    int n = recvfrom(sockfd, mapID, 1024, 0, (struct sockaddr*) &clientaddr, &len);
    mapID[n] = '\0';
    recvfrom(sockfd, &source, sizeof(source), 0, (struct sockaddr*) &clientaddr, &len);
    printf("The Server A has received input for finding shortest paths: starting vertex %d ", source);
    printf("of map %s.\n", mapID);

    Map* targetmap;
    // find map
    for (int i = 0; i < mapcount; i++){
      iter = maps[i];
      //printf("%s\n",mapID );
      //printf("%s\n",*(&iter.m->mapID) );
      if (mapID[0] == *(&iter.m->mapID)[0]){
        //printf("got map %s\n",mapID);
        targetmap = *(&iter.m);
        //printf("%s\n",*(&targetmap->mapID) );
        break;
      }
    }
    // find source index
    int sidx = -1;
    for (int i = 0; i < *(&targetmap->vertex_number); i++){
      //printf("%d,%d\n", i, *(&targetmap->vertrx_sequence[i] ) );
      if ( *(&targetmap->vertrx_sequence[i]) == source ){
        sidx = i;
        //printf("%d\n",sidx );
        break;
      }
    }
    // dijkstra
    int m = *(&targetmap->vertex_number); // number of vetex
    int paths[m]; // destination(index), path length
    int added[m]; // destination(index), index match with adj
    for (int i = 0; i < m; i++) {
      paths[i] = -1;
      added[i] = 0;
    } // initial condition
    paths[sidx] = 0;
    for (int i = 0; i < m; i++){
      // find next to add
      int toadd = -1;
      int min = -1;
      for (int c = 0; c < m; c++){
        if (added[c] == 0 && paths[c] != -1 ){
          if (min == -1 || paths[c] < min){
            toadd = c;
            min = paths[c];
          }
        }
      }
      // updated added and paths after adding toadd
      added[toadd] = 1;
      for (int j = 0; j < m; j++){
        int w = *(&targetmap->adja[toadd][j]);
        int temp = paths[toadd] + w;
        if ( w != 0 && (paths[j] == -1 || paths[j] > temp ) ) paths[j] = temp ;
      }
    }
    //for (int i = 0; i < m; i++) printf("%d\n",paths[i]);
    
    printf("%s\n", "The Server A has identified the following shortest paths:");
    printf("%s\n", "-----------------------------");
    printf("%s\n", "Destination  Min Length");
    printf("%s\n", "-----------------------------");
    for (int i = 0; i < m; i++){
      if (i == sidx) continue;
      printf("%-13d", *(&targetmap->vertrx_sequence[i]));
      printf("%d\n", paths[i]);
    }
    printf("%s\n", "-----------------------------");

    sendto(sockfd, &targetmap->propogation, sizeof(double), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    sendto(sockfd, &targetmap->transmission, sizeof(double), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    sendto(sockfd, &m, sizeof(int), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    for(int i = 0; i < m; i++){
      if (i == sidx) continue;
      sendto(sockfd, &targetmap->vertrx_sequence[i], sizeof(int), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
      sendto(sockfd, &paths[i], sizeof(int), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
      //recvfrom(A_sockfd, paths[i][0], sizeof(int), 0, (struct sockaddr*)NULL, NULL);
      //recvfrom(A_sockfd, paths[i][1], sizeof(int), 0, (struct sockaddr*)NULL, NULL);
    }

    char *message = "Here is A responding";
    sendto(sockfd, message, 1024, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    printf("%s\n", "The Server A has sent shortest paths to AWS.");

  }

}