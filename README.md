# socket_project

0. Your Full Name as given in the class list : Hsueh Wei Hsu

1. Student ID : 2178884539

2. What you have done in the assignment.
	All the requirement in the PDF file and piazza. 

3. What your code files are and what each one of them does.
	client.c : Communicate with aws server using TCP. Send the request, receive the result, print the result.
	aws.c : Communicate with client using TCP. Communicate with Server A and Server B usind UDP. 
		Receive request from client, send request to server A and B, receive result from A and B, print them out and send them back to client.
	serverA.c : Communicate with aws using UDP. Read map file and save the maps information. Reveive request from aws, find the corresponding map. 
		Calculate dijkstra algorithm. print out the result and send required information back to aws.
	serverB.c : Communicate with aws using UDP. Reveive request from aws, calculate the desired result, print them out and send them back to aws.

4. The format of all the messages exchanged.
	client -> aws:
		mapID, filesize : string
		source vertex: int
	aws -> serverA :
		mapID : string
		source vertex: int
	serverA -> aws :
		propogation speed, transimission speed: double
		vertex number, destinations, length of paths: int 
	aws -> serverB :
		filesize: string
		propogation speed, transimission speed: double
		vertex number, destinations, length of paths: int 
	serverB -> aws :
		transimission delay, propogation delays, end to end delays: double
	aws -> client :
		
5. Any idiosyncrasy of your project. It should say under what conditions the project fails, if any.
	No specific condition.

6. Reused Code: Did you use code from anywhere for your project? If not, say so. If so, say what functions and where they're from. (Also identify this with a comment in the source code.)
	Beej's guide for how to use socket.