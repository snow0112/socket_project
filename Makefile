all:
		gcc -o client client.c 
		gcc -o aws aws.c 
		gcc -o serverA serverA.c 
		gcc -o serverB serverB.c 

serverA: 
		./serverA

serverB:
		./serverB

aws:
		./aws