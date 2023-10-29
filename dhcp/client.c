#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<poll.h>
#define PORT 12345

typedef struct frame
{
	char msg[30];
	char yiaddr[15];
	int lifetime;
	int trans_id;
}frame;

void pool()
{
	sleep(5);
}

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
	
	frame f;
	strcpy(f.msg,"DHCPDISCOVER");
	strcpy(f.yiaddr,"0.0.0.0");
	f.trans_id=45;
	f.lifetime=0;
	
	struct pollfd fds[1];
	fds[0].fd=0;
	fds[0].events=POLLIN;
	fds[0].revents=0;
		
	
	printf("DHCP discover...\n");
	sendto(sock,&f,sizeof(f),0,(struct sockaddr*)&sadd,len);
	
	printf("Broadcast to server, yiaddr: %s , trans_id: %d\n",f.yiaddr,f.trans_id);
	
	printf("\nDHCP offer...\n");
	recvfrom(sock,&f,sizeof(f),0,(struct sockaddr*)&sadd,&len);
	printf("Offered ip: %s,Lifetime: %dms\n",f.yiaddr,f.lifetime);
	
	printf("\nDHCP request...\n");
	strcpy(f.msg,"DHCPREQUEST");
	sendto(sock,&f,sizeof(f),0,(struct sockaddr*)&sadd,len);
	printf("Request to server, with ip: %s, trans_id: %d, lifetime: %dms\n",f.yiaddr,f.trans_id,f.lifetime);
	
	printf("\nDHCP acknowledge...\n");
	recvfrom(sock,&f,sizeof(f),0,(struct sockaddr*)&sadd,&len);
	printf("Acknowledged addresss from server: %s, trans_id: %d, Lifetime: %dms\n",f.yiaddr,f.trans_id,f.lifetime);
	
	int timeout=f.lifetime;
	
	while(1){
		pool();
		printf("Enter 1 to renew, 0 to release\n");		
		int activity=poll(fds,1,timeout/2);
		if(activity<=0)
		{
			printf("\nLease time is expired");
			strcpy(f.msg,"DHCPRELEASE");
			sendto(sock,&f,sizeof(f),0,(struct sockaddr*)&sadd,len);
			break;
		}
		scanf("%d",&n);
		if(n==1)
		{
			printf("Renewing\n");
			strcpy(f.msg,"DHCPRENEW");
			sendto(sock,&f,sizeof(f),0,(struct sockaddr*)&sadd,len);
		}
		else
		{
			printf("\nReleasing\n");
			strcpy(f.msg,"DHCPRELEASE");
			sendto(sock,&f,sizeof(f),0,(struct sockaddr*)&sadd,len);
			break;
		}
	}	
	close(sock);
	return 0;
}
