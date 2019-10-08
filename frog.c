#include <bits/stdc++.h>

const int N  = 100005;
const int Q = 100005;

int maze[N][N];
int cost[4] = {3,0,1,0};
int dir[4][2] = {-1,0,0,1,1,0,0,-1};
typedef struct STATE
{
	int x,y,left;
}state;

state queue[Q];

void bfs( int st_x,int st_y,int ed_x,int ed_y,int p)
{
	int countn = 0,head = 0 ,tail = 0;
	state start;
	start.x=st_x,start.y=st_y,start.left=p;
	queue[tail++] = start;
	while( head<tail )
	{
		state tmp = queue[++head];
		if( tmp.left != maze[tmp.x][tmp.y] )
			continue;
		for( int i=0;i<4;i++ )
		{
			if( tmp.x+dir[i][0]<0||tmp.x+dir[i][0]>n||tmp.y+dir[i][1]<0||tmp.y+dir[i][1]>n )
				continue;
			if( tmp.left - cost[i] <= maze[ tmp.x+dir[i][0] ][ tmp.y+dir[i][1] ] )
				continue;
			state npoint;
			npoint.x = tmp.x+dir[i][0],npoint.y = tmp.y+dir[i][0].npoint.left = tmp.left-cost[i];
			queue[tail++] =  npoint;
		}
	}
	if (maze[ed_x][ed_y] > 0)
		return 1;
	return 0;
}
int main()
{
	int n;
	while(~scanf("%d",&n))
	{
		memset(maze,0,sizeof(maze));
		int st_x,st_y,ed_x,ed_y,p;
		for( int i=0;i<n;i++ )
			for( int j=0;j<n;j++ )
				scanf("%d",&maze[i][j]);
		scanf("%d%d%d%d",st_x,st_y,ed_x,ed_y);
		scanf("%d",&p);
		int flag = bfs(st_x,st_y,ed_x,ed_y,p);
		if(flag > 0)
			printf("yes\n");
		else
			printf("no\n");
	}

}
