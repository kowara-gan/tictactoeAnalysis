#include "tictoctoe.h"

#include <unordered_set>
typedef deque<uint64> dUint64;
struct hashUint64{
  int operator()(uint64 const& v) const{
    return (v>>32)^(v&0xffffffffull);
  }
};
typedef unordered_set<uint64,hashUint64> hUint64;

int main()
{
  vUint64 allIS;
  dUint64 q;
  hUint64 v;
  q.push_back(State().getNormalizeBit());
  while(!q.empty()){
    uint64 is=q.front();
    q.pop_front();
    hUint64::const_iterator it=v.find(is);
    if(it==v.end()){
      State s(is,FIRST);
      allIS.push_back(is);
      v.insert(is);
      if(!s.isLoseFirst()){
        //if(true){
	    vUint64 ns=s.nextNormalizeBits();
	    for(size_t i=0;i<ns.size();i++)
        q.push_back(ns[i]);
      }
    }
  }
  cout << v.size() << endl;
  sort(allIS.begin(),allIS.end());
  ofstream ofs("allstates.dat");
  ofs.write((char *)&allIS[0],allIS.size()*sizeof(uint64));
}
