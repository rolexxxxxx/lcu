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
	int sock;
	char buffer[1024];
	struct sockaddr_in sadd,cadd;
	sock=socket(AF_INET,SOCK_DGRAM,0);
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
	socklen_t len;
	len=sizeof(cadd);
	int n;
	for(;;)
	{
		n=recvfrom(sock,buffer,1024,0,(struct sockaddr *)&cadd,&len);
		buffer[n]='\0';
		if(strcmp(buffer,"exit")==0)
		{	
			printf("Client closed");
			break;
		}
		printf("From client: %s\n",buffer);
		printf("To client:");
		scanf("%s",buffer);
		sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&cadd,len);
		if(strcmp(buffer,"exit")==0)
		{	
			break;
		}
	}
	close(sock);
	return 0;
}
