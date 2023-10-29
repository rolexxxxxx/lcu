#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 12347

int gen_len,data_len,i,j;

char gen[4];
char data[20];
char check[20];

void XOR()
{
	for(j=1;j<gen_len;j++)
	{
		check[j]=(check[j]==gen[j])?'0':'1';
	}
}

void crc()
{
	
	for(i=0;i<gen_len;i++)
	{
		check[i]=data[i];
	}
	do{
		if(check[0]=='1')
			XOR();
		for(j=0;j<gen_len-1;j++)
		{
			check[j]=check[j+1];
		}
		check[j]=data[i];
		i++;
		printf("%s\n",check);
	}while(i<data_len+1);
	
}

int main()
{
	int sock,connfd;
	struct sockaddr_in sadd,cadd;
	
	char buffer[1024];
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		perror("Error in socket creation");
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
	
	if(listen(sock,5)!=0){
		perror("Listen Failed");
		exit(-1);
	}
	
	int len=sizeof(cadd);
	
	if((connfd=accept(sock,(struct sockaddr*)&cadd,&len))<0)
	{
		perror("Error in accept");
		exit(-1);
	}
	
	recv(connfd,data,sizeof(data),0);
	recv(connfd,gen,sizeof(gen),0);
	
	printf("Recieved data:%s\n",data);
	
	gen_len=strlen(gen);
	data_len=strlen(data);
	crc();
	
	int flag=1;
	for(i=0;i<strlen(check);i++)
	{
		if(check[i]=='1')
		{
			flag=0;
			break;
		}
	}
	if(flag==0)
		printf("There is an error");
	else{
		printf("There is no error\nCorrect data:");
		for(i=0;i<data_len-gen_len+1;i++)
			printf("%c",data[i]);
	}
}
