#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 12345
int main()
{
	int sock,n;
	char buffer[1024];
	struct sockaddr_in sadd;
	sock=socket(AF_INET,SOCK_DGRAM,0);
	if(sock<0)
	{
		perror("error insocket");
		exit(-1);
	}
	printf("Socket created successfully\n");
	sadd.sin_family=AF_INET;
	sadd.sin_port=htons(PORT);
	sadd.sin_addr.s_addr=inet_addr("127.0.0.1");
	//inet_pton(AF_INET,"127.0.0.1",&sadd.sin_addr);
	socklen_t len;
	len=sizeof(sadd);
	for(;;)
	{
		printf("To server:");
		scanf("%s",buffer);
		sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&sadd,len);
		if(strcmp(buffer,"exit")==0)
		{	
			break;
		}
		n=recvfrom(sock,buffer,1024,0,(struct sockaddr *)&sadd,&len);
		buffer[n]='\0';
		if(strcmp(buffer,"exit")==0)
		{	
			printf("Server closed");
			break;
		}
		printf("From server: %s\n",buffer);
	}
	close(sock);
	return 0;
}
