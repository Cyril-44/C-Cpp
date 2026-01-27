#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
const int INF=1e9;
int map[5005][5005]={0};
int Gap[5005]={0},dis[5005]={0};
int n,m,Ans=0;
int DFS(int x,int num)
{
	if(x==n)return num;
	int t=0,s=0;
	for(int i=1;i<=n;i++)
	{
		if(map[x][i]>0&&dis[x]==dis[i]+1)
		{
			s=DFS(i,min(num,map[x][i]));
			num-=s;t+=s;
			map[x][i]-=s;map[i][x]+=s;
			if(dis[0]==n+2||!num)return t;
		}
	}
	if(!(--Gap[dis[x]]))dis[1]=n+2;Gap[++dis[x]]++;
	return t;
}
int main()
{
	int x,y,z;
	scanf("%d%d",&m,&n);
	for(int i=1;i<=m;i++)
	{
		scanf("%d%d%d",&x,&y,&z);
		map[x][y]+=z;
	}
	Gap[0]=n;
	while(dis[1]<n)Ans+=DFS(1,INF);
	printf("%d\n",Ans);
	return 0;
}
