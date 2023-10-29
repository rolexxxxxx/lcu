#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>


#define PORT 12345

typedef struct frame
{
	char msg[30];
	char yiaddr[15];
	int lifetime;
	int trans_id;
}frame;


char ip[5][15]={"41.120.123.1","41.120.123.2","41.120.123.3","41.120.123.4","41.120.123.5"};
int flag[5]={0,0,0,0,0};

int findip()
{
	for(int i=0;i<5;i++)
	{
		if(flag[i]==0)
		{
			flag[i]=1;
			return i;
		}
	}
	return -1;
}

int main()
{
	int sock;
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
	int yip;
	
	frame f;
	
	
	for(;;)
	{
		
		recvfrom(sock,&f,sizeof(f),0,(struct sockaddr*)&cadd,&len);
		
		if(strcmp(f.msg,"DHCPDISCOVER")==0)
		{
			printf("DHCP discover...\n");
			printf("Broadcast from client, yiaddr: %s , trans_id: %d\n",f.yiaddr,f.trans_id);
			
			printf("\nDHCP offer...\n");
			strcpy(f.msg,"DHCPOFFER");
			yip=findip();
			if(yip==-1)
			{
				printf("No ip address available");
				break;
			}
			strcpy(f.yiaddr,ip[yip]);
			f.lifetime=10000;
			sendto(sock,&f,sizeof(f),0,(struct sockaddr*)&cadd,len);
			printf("Offered ip: %s,Lifetime: %dms\n",f.yiaddr,f.lifetime);
			
			printf("\nDHCP request...\n");
			recvfrom(sock,&f,sizeof(f),0,(struct sockaddr*)&cadd,&len);
			
			printf("Request from client, with ip: %s, trans_id: %d, lifetime: %dms\n",f.yiaddr,f.trans_id,f.lifetime);
			
			printf("\nDHCP acknowledge...\n");
			strcpy(f.msg,"DHCPACK");
			sendto(sock,&f,sizeof(f),0,(struct sockaddr*)&cadd,len);
			
			printf("Acknowledged address: %s, trans_id: %d, Lifetime: %dms\n",f.yiaddr,f.trans_id,f.lifetime);
			
		}
		
		while(1)
		{
			recvfrom(sock,&f,sizeof(f),0,(struct sockaddr*)&cadd,&len);
			if(strcmp(f.msg,"DHCPRENEW")==0)
			{
				printf("Renewing Client with IP:%s\n",f.yiaddr);
			}
			else if(strcmp(f.msg,"DHCPRELEASE")==0)
			{
				printf("Client with IP:%s terminated or lease time expired\n",f.yiaddr);
				flag[yip]=0;
				break;
			}
		}
	
			
		
	}
	close(sock);
	return 0;
}
