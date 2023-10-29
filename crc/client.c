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
	
		
	}while(i<data_len+gen_len);
	
}


int main()
{
	int sock;
	struct sockaddr_in sadd;
	
	char buffer[1024];
	
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		perror("Error in socket creation");
		exit(-1);
	}
	printf("Socket created successfully\n");
	
	sadd.sin_family=AF_INET;
	sadd.sin_addr.s_addr=inet_addr("127.0.0.1");
	sadd.sin_port=htons(PORT);
	int len=sizeof(sadd);
	if(connect(sock,(struct sockaddr *)&sadd,len)!=0)
	{
		printf("Connection with the server failed");
		exit(0);
	}
	
	printf("Enter data:");
	scanf("%s",data);
	printf("Enter generator polynomial:");
	scanf("%s",gen);
	
	gen_len=strlen(gen);
	data_len=strlen(data);
	for(i=data_len;i<data_len+gen_len-1;i++)
	{
		data[i]='0';
	}
	
	crc();
	
	printf("Data padded with n zeros: %s\n",data);
	j=0;
	for(i=data_len;i<data_len+gen_len-1;i++)
	{
		data[i]=check[j];
		j++;
	}
	
	printf("Data to be sent: %s\n",data);
	
	send(sock,data,sizeof(data),0);
	send(sock,gen,sizeof(gen),0);
	sleep(5);
		
}
