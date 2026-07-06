#include <vector>

extern "C" {

void human_exe(int tid,int n,int u,int v,int w,int a,int b,int c,std::vector<int> &f);
void finish(int tid,int n,std::vector<int> f);
void attention_is_all_you_need(int tid,int n,std::vector<int> u,std::vector<int> v,std::vector<int> w,std::vector<int> a,std::vector<int> b,std::vector<int> c);
int report_edge(int u,int v,int w);
void answer(int x,int y);
void set_n(int n);

}