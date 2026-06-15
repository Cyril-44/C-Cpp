#ifndef CIRCLE_H
#define CIRCLE_H

#define GRADER   // 如果选手需要使用 grader，请保留这一行，并且注释掉第5行代码
//#define CHECKER  // 如果选手需要使用 checker，请保留这一行，并且注释掉第4行代码

#include<bits/stdc++.h>
using namespace std;
struct info{
	unsigned long long hash;
	int vertex[2];
};
const info emptyinfo=info{0,-1,-1};
info MR(info a,info b);
info MC(info a,info b);
void init(int T,int n,int q,vector<int> fa,vector<info> e,int M);
bool isempty(info a);
info ask(int x,int d);
#endif
