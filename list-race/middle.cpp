#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <cstdio>
#include <algorithm>
using namespace std; using clk=chrono::steady_clock; volatile size_t sink;
double since(clk::time_point t){return chrono::duration<double,milli>(clk::now()-t).count();}
int main(){ const int M=1000000,K=10000;
 for(int rep=0;rep<5;rep++){
  vector<int> v(M,1); auto t=clk::now(); for(int i=0;i<K;i++) v.insert(v.begin()+v.size()/2,i); double dv=since(t);
  list<int> l(M,1); t=clk::now(); for(int i=0;i<K;i++){auto it=l.begin(); advance(it,l.size()/2); l.insert(it,i);} double dl=since(t);
  list<int> a(M,1); vector<list<int>::iterator> its; for(auto it=a.begin();it!=a.end();++it) its.push_back(it); mt19937 r(rep); shuffle(its.begin(),its.end(),r); list<int> s; for(auto it:its) s.splice(s.end(),a,it);
  const int KS=500; t=clk::now(); for(int i=0;i<KS;i++){auto it=s.begin(); advance(it,s.size()/2); s.insert(it,i);} double ds=since(t);
  printf("vec %.0f ms | list-inorder %.0f ms (x%.1f) | list-scattered %.1f ms/insert vs vec %.4f ms/insert (x%.0f)\n",dv,dl,dl/dv,ds/KS,dv/K,(ds/KS)/(dv/K));
  sink+=v.size()+l.size()+s.size(); }
}
