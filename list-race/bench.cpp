#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <numeric>
using namespace std;
using clk = chrono::steady_clock;
static double ms(clk::time_point a, clk::time_point b){return chrono::duration<double,milli>(b-a).count();}
volatile uint64_t sink;

template<class C> double stroustrup(int n, uint32_t seed){
  mt19937 rng(seed); C c;
  auto t0=clk::now();
  for(int i=0;i<n;i++){ int v=rng(); auto it=c.begin(); while(it!=c.end() && *it<v) ++it; c.insert(it,v); }
  for(int i=0;i<n;i++){ size_t k=rng()%c.size(); auto it=c.begin(); advance(it,k); c.erase(it); }
  auto t1=clk::now(); sink+=c.size(); return ms(t0,t1);
}
// insert K times into the middle of a container of M elements
double vec_mid(int M,int K){ vector<int> v(M,1); auto t0=clk::now(); for(int i=0;i<K;i++) v.insert(v.begin()+v.size()/2,i); auto t1=clk::now(); sink+=v.size(); return ms(t0,t1);}
double list_mid_walk(int M,int K){ list<int> l(M,1); auto t0=clk::now(); for(int i=0;i<K;i++){ auto it=l.begin(); advance(it,l.size()/2); l.insert(it,i);} auto t1=clk::now(); sink+=l.size(); return ms(t0,t1);}
double list_mid_ptr(int M,int K){ list<int> l(M,1); auto it=l.begin(); advance(it,M/2); auto t0=clk::now(); for(int i=0;i<K;i++) l.insert(it,i); auto t1=clk::now(); sink+=l.size(); return ms(t0,t1);}

// pointer chase: nodes of 128 bytes (one cache line on M4), random or sequential cycle
struct alignas(128) Node{ Node* next; char pad[120]; };
double chase(size_t bytes, bool rnd){
  size_t n=bytes/sizeof(Node); vector<Node> a(n); vector<size_t> p(n); iota(p.begin(),p.end(),0);
  if(rnd){ mt19937_64 r(42); shuffle(p.begin()+1,p.end(),r);} 
  for(size_t i=0;i<n;i++) a[p[i]].next=&a[p[(i+1)%n]];
  size_t hops=max<size_t>(20'000'000, n*2); Node* x=&a[0];
  for(size_t i=0;i<n;i++) x=x->next; // warm
  auto t0=clk::now(); for(size_t i=0;i<hops;i++) x=x->next; auto t1=clk::now();
  sink+=(uintptr_t)x; return ms(t0,t1)*1e6/hops;
}
int main(){
  printf("== Stroustrup: insert N random ints sorted (linear search), then erase random positions ==\n");
  for(int n: {1000, 5000, 10000, 20000, 50000, 100000}){
    double v=stroustrup<vector<int>>(n,1), l=stroustrup<list<int>>(n,1);
    printf("N=%7d  vector %9.1f ms   list %9.1f ms   list/vector = %.1fx\n",n,v,l,l/v);
  }
  printf("\n== insert into the middle, K=10000 times, start M elements ==\n");
  for(int M: {1000, 10000, 100000, 1000000}){
    printf("M=%8d  vector %9.1f ms   list(walk to middle) %9.1f ms   list(have pointer) %7.3f ms\n",M,vec_mid(M,10000),list_mid_walk(M,10000),list_mid_ptr(M,10000));
  }
  printf("\n== pointer chase ns per hop ==\n");
  for(size_t kb: {32ul, 256ul, 4096ul, 16384ul, 65536ul, 524288ul}){
    printf("%8zu KB  sequential %6.2f ns   random %7.2f ns\n",kb,chase(kb*1024,false),chase(kb*1024,true));
  }
}
