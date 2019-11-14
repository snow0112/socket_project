all: serverA.c serverB.c aws.c client.c
		gcc -o client client.c 
		gcc -o aws aws.c 
		gcc -o serverA serverA.c 
		gcc -o serverB serverB.c 

serverA: serverA.o
		./serverA

serverB: serverB.o
		./serverB

aws: aws.o
		./aws

clean: 
	$(RM) serverA
	$(RM) serverB
	$(RM) aws
	$(RM) client
	$(RM) serverA.o
	$(RM) serverB.o
	$(RM) aws.o
	$(RM) client.o