#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define PORT 12345
int main()
{
	int sock,connfd;
	char buffer[1024];
	struct sockaddr_in sadd,cadd;
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("error insocket");
		exit(-1);
 	}
	printf("Socket created successfully\n");
	sadd.sin_family=AF_INET;
	sadd.sin_addr.s_addr=htonl(INADDR_ANY);
	sadd.sin_port=htons(PORT);	
	if(bind(sock,(struct sockaddr*)&sadd,sizeof(sadd))<0)
	{
		perror("Error in binding");
		exit(-1);
	}
	if(listen(sock,1)!=0)
	{
		printf("Listen Failed");
		exit(-1);
	}
	socklen_t len;
	len=sizeof(cadd);
	connfd=accept(sock,(struct sockaddr *)&cadd,&len);
	if(connfd<0)
	{
		printf("Server accept failed");
		exit(0);
	}
	int n;
	for(;;)
	{
		recv(connfd,buffer,sizeof(buffer),0);
		printf("From client: %s",buffer);
		if(strncmp(buffer,"exit",4)==0)
		{	
			printf("Client closed");
			break;
		}
		printf("To client:");
		scanf("%s",buffer);
		send(connfd,buffer,sizeof(buffer),0);
		if(strncmp(buffer,"exit",4)==0)
		{	
			break;
		}
	}
	close(sock);
	return 0;
}
