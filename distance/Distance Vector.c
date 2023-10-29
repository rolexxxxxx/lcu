#include<stdio.h>
#include<stdlib.h>

int main()
{
	/*int cost[7][7]={{0,1,1,999,1,1,999},
				{1,0,1,999,999,999,999},
				{1,1,0,1,999,999,999},
				{999,999,1,0,999,999,1},			
				{1,999,999,999,0,999,999},
				{1,999,999,999,999,0,1},
				{999,999,999,1,999,1,0}};
				
	for(int i=0;i<7;i++)
	{
		for(int j=0;j<7;j++)
		{
			for(int k=0;k<7;k++)
			{
				if(mat[i][k]+mat[k][j]<mat[i][j])
				{
					mat[i][j]=mat[i][k]+mat[k][j];	
				}	
			}	
		}
	}*/
	
	FILE *f=fopen("edges.txt","r");
	char buf[10];
	
	int n,e,s,d,c;
	fscanf(f,"%d",&n);
	printf("the number of nodes: %d\n",n);
	int cost[n][n];
	int dis[n][n];
	
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(i==j){
				dis[i][j]=0;
				cost[i][j]=0;
			}
			else{
				dis[i][j]=999;
				cost[i][j]=999;
			}			
		}
	}
	
	fscanf(f,"%d",&e);		
	printf("the number of edges: %d\n",e);	
		
	for(int i=0;i<e;i++)
	{
		
		fscanf(f,"%d %d %d",&s,&d,&c);
		cost[s][d]=c;
		cost[d][s]=c;	
	}	
	/*for(int i=0;i<n;i++)
	{
		for(int j=0;j<n-1;j++)
		{
			for(int k=0;k<n;k++)
			{
				for(int l=0;l<n;l++)
				{
					if(cost[l][k]!=999)
					{
						if(dis[i][l]+cost[l][k]<dis[i][k])
						{
							dis[i][k]=dis[i][l]+cost[l][k];
						}
					}	
				}	
			}	
		}
	}*/
	for(int i=0;i<n;i++)
	{
		printf("For node %d\n\n",i);
		for(int j=0;j<n-1;j++)
		{
			int flag=0;
			fseek(f,0,SEEK_SET);
			
			fgets(buf,10,f);
			fgets(buf,10,f);
			for(int k=0;k<e;k++)
			{
				fscanf(f,"%d %d %d",&s,&d,&c);				
				if(dis[i][s]+cost[s][d]<dis[i][d])
				{
					flag=1;
					dis[i][d]=dis[i][s]+cost[s][d];
				}
				if(dis[i][d]+cost[d][s]<dis[i][s])
				{
					flag=1;
					dis[i][s]=dis[i][d]+cost[d][s];
				}					
			}
			if(flag==1)
			{
				printf("Round:%d\n",j);
				for(int l=0;l<7;l++)
				{
					printf("%d\t",dis[i][l]);
				}
				printf("\n\n");
			}
			else{
				break;
			}
				
		}
	}
	for(int i=0;i<7;i++)
	{
		for(int j=0;j<7;j++)
		{
			printf("%d\t",dis[i][j]);
		}
		printf("\n");
	}
}
