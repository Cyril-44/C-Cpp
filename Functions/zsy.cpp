#include <bits/stdc++.h>
#include <unistd.h>
#define Sleep sleep
using namespace std;

int yanchi, n, m, wx, wy, o, p, ffo, poi, jkl, po;
string y[1005][1005], k; 
int sx, sy, zx, zy, gx[1005], gy[1005], tx, ty;
int d[1005][1005];
int dx[5] = {0, 0, 1, 0, -1};
int dy[5] = {0, 1, 0, -1, 0};
void gh(int i)
{
	if (i == 1) 
	{
		dx[1] = 0; dx[2] = 1; dx[3] = 0; dx[4] = -1;
		dy[1] = 1; dy[2] = 0; dy[3] = -1; dy[4] = 0; 
	}
	if (i == 2) 
	{
		dx[1] = 0; dx[2] = 1; dx[4] = 0; dx[3] = -1;
		dy[1] = 1; dy[2] = 0; dy[4] = -1; dy[3] = 0; 
	}
	if (i == 3) 
	{
		dx[1] = 0; dx[3] = 1; dx[2] = 0; dx[4] = -1;
		dy[1] = 1; dy[3] = 0; dy[2] = -1; dy[4] = 0; 
	}
	if (i == 4) 
	{
		dx[1] = 0; dx[3] = 1; dx[4] = 0; dx[2] = -1;
		dy[1] = 1; dy[3] = 0; dy[4] = -1; dy[2] = 0; 
	}
	if (i == 5) 
	{
		dx[1] = 0; dx[4] = 1; dx[2] = 0; dx[3] = -1;
		dy[1] = 1; dy[4] = 0; dy[2] = -1; dy[3] = 0; 
	}
	if (i == 6) 
	{
		dx[1] = 0; dx[4] = 1; dx[3] = 0; dx[2] = -1;
		dy[1] = 1; dy[4] = 0; dy[3] = -1; dy[2] = 0; 
	}
	if (i == 7) 
	{
		dx[2] = 0; dx[1] = 1; dx[3] = 0; dx[4] = -1;
		dy[2] = 1; dy[1] = 0; dy[3] = -1; dy[4] = 0; 
	}
	if (i == 8) 
	{
		dx[2] = 0; dx[1] = 1; dx[4] = 0; dx[3] = -1;
		dy[2] = 1; dy[1] = 0; dy[4] = -1; dy[3] = 0; 
	}
	if (i == 9) 
	{
		dx[2] = 0; dx[3] = 1; dx[1] = 0; dx[4] = -1;
		dy[2] = 1; dy[3] = 0; dy[1] = -1; dy[4] = 0; 
	}
	if (i == 10) 
	{
		dx[2] = 0; dx[3] = 1; dx[4] = 0; dx[1] = -1;
		dy[2] = 1; dy[3] = 0; dy[4] = -1; dy[1] = 0; 
	}
	if (i == 11) 
	{
		dx[2] = 0; dx[4] = 1; dx[1] = 0; dx[3] = -1;
		dy[2] = 1; dy[4] = 0; dy[1] = -1; dy[3] = 0; 
	}
	if (i == 12) 
	{
		dx[2] = 0; dx[4] = 1; dx[3] = 0; dx[1] = -1;
		dy[2] = 1; dy[4] = 0; dy[3] = -1; dy[1] = 0; 
	}
	if (i == 13) 
	{
		dx[3] = 0; dx[2] = 1; dx[1] = 0; dx[4] = -1;
		dy[3] = 1; dy[2] = 0; dy[1] = -1; dy[4] = 0; 
	}
	if (i == 14) 
	{
		dx[3] = 0; dx[2] = 1; dx[4] = 0; dx[1] = -1;
		dy[3] = 1; dy[2] = 0; dy[4] = -1; dy[1] = 0; 
	}
	if (i == 15) 
	{
		dx[3] = 0; dx[1] = 1; dx[2] = 0; dx[4] = -1;
		dy[3] = 1; dy[1] = 0; dy[2] = -1; dy[4] = 0; 
	}
	if (i == 16) 
	{
		dx[3] = 0; dx[1] = 1; dx[4] = 0; dx[2] = -1;
		dy[3] = 1; dy[1] = 0; dy[4] = -1; dy[2] = 0; 
	}
	if (i == 17) 
	{
		dx[3] = 0; dx[4] = 1; dx[2] = 0; dx[1] = -1;
		dy[3] = 1; dy[4] = 0; dy[2] = -1; dy[1] = 0; 
	}
	if (i == 18) 
	{
		dx[3] = 0; dx[4] = 1; dx[1] = 0; dx[2] = -1;
		dy[3] = 1; dy[4] = 0; dy[1] = -1; dy[2] = 0; 
	}
	if (i == 19) 
	{
		dx[4] = 0; dx[2] = 1; dx[3] = 0; dx[1] = -1;
		dy[4] = 1; dy[2] = 0; dy[3] = -1; dy[1] = 0; 
	}
	if (i == 20) 
	{
		dx[4] = 0; dx[2] = 1; dx[1] = 0; dx[3] = -1;
		dy[4] = 1; dy[2] = 0; dy[1] = -1; dy[3] = 0; 
	}
	if (i == 21) 
	{
		dx[4] = 0; dx[3] = 1; dx[2] = 0; dx[1] = -1;
		dy[4] = 1; dy[3] = 0; dy[2] = -1; dy[1] = 0; 
	}
	if (i == 22) 
	{
		dx[4] = 0; dx[3] = 1; dx[1] = 0; dx[2] = -1;
		dy[4] = 1; dy[3] = 0; dy[1] = -1; dy[2] = 0; 
	}
	if (i == 23) 
	{
		dx[4] = 0; dx[1] = 1; dx[2] = 0; dx[3] = -1;
		dy[4] = 1; dy[1] = 0; dy[2] = -1; dy[3] = 0; 
	}
	if (i == 24) 
	{
		dx[4] = 0; dx[1] = 1; dx[3] = 0; dx[2] = -1;
		dy[4] = 1; dy[1] = 0; dy[3] = -1; dy[2] = 0; 
	}
}
void cq()
{
	system("clear");
	yanchi = n = m = wx = wy = o = p = jkl = po = 0;
	ffo = poi = 1;
	for (int i = 0; i <= 1004; i++)
		for (int j = 0; j <= 1004; j++)
			{y[i][j] = ""; d[i][j] = 0;}
	k = "";
	for (int i = 0; i <= 1004; i++)
		gx[i] = gy[i] = 0;
	sx = sy = zx = zy = tx = ty = 0;
}
bool pd(int i, int j, int u)
{
	if (u == 1)
	{
		if (y[i][j] == "墙" || y[i][j] == "专") return 0;
		return 1;
	}
	else
	{
		if (y[i][j] == "赢") return 1;
		return 0;
	}
}

void print(string s)
{
	while (s.size() != 0)
	{
		printf("%c", s[0]);
		s.erase(0, 1);
		Sleep(yanchi);
	}
}
void sc()
{
	for (int i = 0; i <= n + 1; i++)
	{
		for (int j = 0; j <= m + 1; j++)
		{
			int pq = 1;
			for (int k = 1; k < ffo; k++)
				if (i == gx[k] && j == gy[k])
				{
					printf("\033[37m"); 
					print("鬼");
					pq = 0;
					break;
				}
			if (i == wx && j == wy && pq == 1) 
			{
				
				if (jkl) {printf("\033[33m"); print("胜");}
				else {printf("\033[36m"); print("我");} 
			}
			else if (i >= wx - o && i <= wx + o && j >= wy - o && j <= wy + o && pq == 1)
			{
				if (y[i][j] == "行") printf("\033[34m"); 
				if (y[i][j] == "赢") printf("\033[31m"); 
				if (y[i][j] == "墙") printf("\033[35m"); 
				if (y[i][j] == "路") printf("\033[32m");
				if (y[i][j] == "专") printf("\033[32m");
				if (y[i][j] == "禁") printf("\033[31m");
				print(y[i][j]);
			}
			else if (pq == 1)
			{
				if (y[i][j] == "行") printf("\033[34m");
				else printf("\033[30m");
				print("行");
			}
			Sleep(yanchi);
		}
		printf("\n");
		Sleep(yanchi);
	} 
	printf("\033[37m"); 
}

void dg(int dep, int x, int y, int xx, int yy, string s)
{
	if (dep == 0)
	{
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= m; j++)
				d[i][j] = INT_MAX;
		gh((rand() % 24 + 1));
		po = 0;
	}
	if (dep < d[x][y])
	{
		if (po == 0) k = s;
		d[x][y] = dep;
		if (pd(x, y, 2) && po != 2) {po = 1; k = s;} 
		if (x == xx && y == yy) {k = s; po = 2; return ;}
		for (int k = 1; k <= 4; k++)
		{
			string ll = "墙"; 
			int i = x + dx[k];
			int j = y + dy[k];
			if (pd(i, j, 1)) dg(dep + 1, i, j, xx, yy, s + char(k + '0'));
		}
	}
}
bool ppdd()
{
	for (int i = 1; i < ffo; i++)
		if (gx[i] != 1000) return 0;
	return 1;
}

int main()
{
	srand(time(0));
	while(1)
	{
		cq();
		//一阶段：延迟。 
		print("你好，这里是迷宫大闯关游戏，想见识见识吗，想就跟我来吧！\n");
		print("现在先请你设定一下游戏参数。\n");
		print("参数设定。\n延迟：几毫秒？\n");
		cin >> yanchi;
		print("OK，延迟设置完毕，以后我都按这个时间来算延迟了。\n"); 
		system("clear");
		//二阶段：长与宽的输入。
		print("现在请设计一个长为n，宽为m的矩阵。\n我设计长为：");
		cin >> n;
		Sleep(yanchi);
		print("宽为：");
		cin >> m;
		Sleep(yanchi);
		system("clear");
		//三阶段：反复确认，输入模块。 
		print("OK，你选择了长为");
		printf("%d", m);
		Sleep(yanchi);
		print("，宽为"); 
		printf("%d", n);
		Sleep(yanchi);
		print("的矩阵。\n");
		system("clear");
		print("现在，请输入长为");
		printf("%d", n);
		Sleep(yanchi);
		print("，宽为"); 
		printf("%d", m);
		Sleep(yanchi);
		print("的矩阵吧。\n在这里，6为只有鬼才能走的路，5为我的专用道，4为鬼，3为终点（最多1个），2为起点（最多1个），1为墙，0为公共道路。\n"); 
		for (int i = 0; i <= n + 1; i++)
			for (int j = 0; j <= m + 1; j++)
				y[i][j] = "墙";
		for (int i = 1; i <= n; i++)
		{
			getchar();
			for (int j = 1; j <= m; j++)
			{
				char x; 
				cin >> x;
				int z = x - '0';
				if (z == 0) y[i][j] = "路";
				else if (z == 1) y[i][j] = "墙";
				else if (z == 2) 
				{
					y[i][j] = "路";
					wx = i, wy = j; 
				}
				else if (z == 3) {y[i][j] = "赢"; poi = 0;}
				else if (z == 4) 
				{
					y[i][j] = "路"; 
					gx[ffo] = i, gy[ffo] = j; 
					ffo++; 
				}
				else if (z == 5) y[i][j] = "专"; 
				else y[i][j] = "禁"; 
			}	
		}
		system("clear");
		//四阶段：游戏难度。 
		o = 9999999; 
		print("请问是否开启记录功能？（1：yes 0：no）\n");
		cin >> p;
		//五阶段：游戏开始。
		print("参数设置完毕，游戏正式开始。");
		if (wx == 0)
		{
			y[0][0] = "墙";
			jkl = 1; 
		}
			
		if (poi)
		{
			y[wx][wy] = "路";
			wx = 0; wy = 0;
			y[0][0] = "墙"; 
			jkl = 1;
		}
		while(1)
		{
			system("clear");
			int u = wx, d = wy, uu = 0;
			for (int i = 1; i < ffo; i++)
			{
				if (gx[i] == wx && gy[i] == wy && !jkl)
				{
					sc();
					printf("\033[31m");
					print("你被鬼杀死了，挑战失败！\n"); 
					printf("\033[30m");
					return 0;
				}
				else if (gx[i] == wx && gy[i] == wy && jkl)
				{
					uu++;
					gx[i] = 1000; gy[i] = 1000;
				}
			}
			if (uu != 0)
			{
				printf("\033[31m");
				print("你把");
				printf("%d", uu);
				print("只鬼杀死了，不错！\n"); 
				printf("\033[37m");
			}
			if (ppdd() && jkl) 
				break;
			sc();
			print("你要向哪一个方向走呢？\na：←； d：→； w：↑； s：↓；\n");
			getchar();
			string s;
			cin >> s;
			int f = 0, y1 = 0;
			if (s == "a")
			{
				if (y[wx][wy - 1] == "赢") 
				{
					jkl = 1;
					y1 = 1;
					wy--;
					y[wx][wy] = "路"; 
				}
				else if ((y[wx][wy - 1] == "墙" || y[wx][wy - 1] == "禁") && !jkl) f = 1;
				else wy--;
			} 
			else if (s == "w")
			{
				if (y[wx - 1][wy] == "赢") 
				{
					jkl = 1;
					y1 = 1;
					wx--;
					y[wx][wy] = "路"; 
				}
				else if ((y[wx - 1][wy] == "墙" || y[wx - 1][wy] == "禁") && !jkl) f = 1;
				else wx--;
			}
			else if (s == "d")
			{
				if (y[wx][wy + 1] == "赢") 
				{
					jkl = 1;
					wy++;
					y1 = 1;
					y[wx][wy] = "路"; 
				}
				else if ((y[wx][wy + 1] == "墙" || y[wx][wy + 1] == "禁") && !jkl) f = 1;
				else wy++;
			}
			else
			{
				if (y[wx + 1][wy] == "赢") 
				{
					jkl = 1;
					wx++;
					y1 = 1;
					y[wx][wy] = "路"; 
				}
				else if ((y[wx + 1][wy] == "墙" || y[wx + 1][wy] == "禁") && !jkl) f = 1;
				else wx++;
			}
			if (y1)
			{
				if (p) y[u][d] = "行"; 
				system("clear");
				o = max(n, m) + 2;
				sc();
				printf("\033[31m");
				print("恭喜你，闯关成功！\n"); 
				printf("\033[30m");
			}
			else if (f) 
			{
				print("当前位置为墙，不可通行。");
				Sleep(50);
			} 
			else
			{
				if (p && jkl == 0 && y[u][d] != "专") y[u][d] = "行";
				else if (jkl == 1) y[u][d] = "专";
			}
			for (int iii = 1; iii < ffo; iii++)
			{
				k = "";
				dg(0, gx[iii], gy[iii], wx, wy, k);
				k += '0';
				int kk = k[0] - '0';
				gx[iii] += dx[kk]; gy[iii] += dy[kk];
			}
		}
		Sleep(1000);
		sc(); 
		printf("\033[31m");
		print("恭喜你，把所有鬼杀光了，获得游戏胜利！\n"); 
		printf("\033[37m");
	}
	return 0;
}
