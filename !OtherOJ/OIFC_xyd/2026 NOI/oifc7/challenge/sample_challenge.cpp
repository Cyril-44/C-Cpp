#include "challenge.h"
#include <bits/stdc++.h>

extern "C" {

void human_exe(int n,int u,int v,int w,int a,int b,int c,std::vector<int> &f) {
	int res=report_edge(1,1,1);
	if(res!=-1) {
		answer(1,0);
	}
}

void finish(int n,std::vector<int> f) {
	int res=report_edge(1,1,1);
	if(res!=-1) {
		answer(1,0);
	}
	set_n(0);
}

void attention_is_all_you_need(int n,std::vector<int> u,std::vector<int> v,std::vector<int> w,std::vector<int> a,std::vector<int> b,std::vector<int> c) {
	int res=report_edge(1,1,1);
	if(res!=-1) {
		answer(1,0);
	}
	set_n(0);
}

}