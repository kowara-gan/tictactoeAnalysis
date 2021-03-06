#include "winLoseTable.h"
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>

WinLoseTable::WinLoseTable(AllStateTable const& allS_,string const& wlName, string const& wlcName,bool lazy)
  :allS(allS_)
{
  struct stat st1,st2;
  if(stat(wlName.c_str(),&st1)<0 || stat(wlcName.c_str(),&st2)<0){
    assert(0);
  }
  if(allS.size() != st1.st_size || st1.st_size != st2.st_size){
    std::cerr << "File size miss match " << wlName << " <=> " << wlcName << std::endl;
    assert(0);
  }
  ifs1.open(wlName.c_str());
  ifs2.open(wlcName.c_str());
  if(!lazy){
    winLose.resize(st1.st_size);
    winLoseCount.resize(st2.st_size);
    ifs1.read((char *)&winLose[0],st1.st_size);
    ifs2.read((char *)&winLoseCount[0],st2.st_size);
    ifs1.close();
    ifs2.close();
  }
}

WinLoseTable::~WinLoseTable(){
  if(winLose.size()==0){
    ifs1.close();
  }
  if(winLoseCount.size()==0){
    ifs2.close();
  }
}


int WinLoseTable::getWL(uint64 v,int& wlc) const
{
  int index=allS.find(v);
  wlc = (int)getWinLoseCount(index);
  return (int)getWinLose(index);
}

void draw(uint64 a){
  State s(a,true);
  s.draw();
}

void WinLoseTable::showSequence(State const& s) const
{
  cin.exceptions(ios::failbit);
  while(true){
  uint64 v=s.getNormalizeBit();
  int index=allS.find(v);
  std::cerr << "------------------" << std::endl;
  std::cerr << s.getStateNum() << std::endl;
  s.isFirst()? std::cerr << "TURNofX " : std::cerr << "TURNofO " ;
  std::cerr << (int)(s.turn==FIRST ? getWinLose(index) : -getWinLose(index)) <<
    "(" << (int)getWinLoseCount(index) << ")" << std::endl;
            vUint64 ns=s.nextNormalizeBits();
            vUint64 shows=s.nextStateNums();
            if(ns.size()==0)break;
            int i=1;
            for(auto &a : ns){
              int wl,wlc;
              wl=getWL(a,wlc);
              std::cerr << i << " : " << shows[i-1] << " "
            << -wl << "(" << wlc << ")" << std::endl;
            draw(shows[i-1]);
            i++;
            }
            uint64 ia;
            try{
              cin>>ia;
            }catch(...){
              cin.clear();
              cin.seekg(0);
              continue;
            }
            if(ia==0)break;
            State news(ia,true);showSequence(news);
            break;
        }
    
}
