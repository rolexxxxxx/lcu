#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 23456
#define SWS 3

typedef struct{
	int seq_no;
	char data[20];
	int ack;
}Frame;

int main()
{
	int sock,connfd;
	struct sockaddr_in sadd;
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("error insocket");
		exit(-1);
	}
	printf("Socket created successfully\n");
	
	sadd.sin_family=AF_INET;
	sadd.sin_port=htons(PORT);
	sadd.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	socklen_t len;
	len=sizeof(sadd);
	if(connect(sock,(struct sockaddr *)&sadd,len)!=0)
	{
		printf("Connection with the server failed");
		exit(0);
	}
	
	
	int MaxSeqNo=8;
	Frame frames[MaxSeqNo];
	int LFS=-1,LAR=-1,i=LAR+1,flag=1,j;
	
	for(;;)
	{
		for(j=0;j<SWS;j++)
		{
			
			frames[i].seq_no=i;
			printf("Enter message for frame %d:",i);
			scanf("%s",frames[i].data);			
			
			send(sock,&frames[i],sizeof(Frame),0);
			
			LFS=i;
			i=(i+1)%MaxSeqNo;
		}
		
		i=(LAR+1)%MaxSeqNo;
		
		for(j=0;j<SWS;j++)
		{
			recv(sock,&frames[i].ack,sizeof(int),0);
			i=(i+1)%MaxSeqNo;
		}
		i=(LAR+1)%MaxSeqNo;
		for(j=0;j<SWS;j++)
		{
			
			if(frames[i].ack==0)
			{	
							
				flag=0;
				break;
			}
			LAR+=1;
			flag=1;
			i=(i+1)%MaxSeqNo;
			
		}
		
		if(flag==0){
			printf("Retransmit frame!\n");
			continue;
		}
		
		i=(LAR+1)%MaxSeqNo;
		printf("\n");
		
		
	}
	
	close(sock);
	return 0;
}
