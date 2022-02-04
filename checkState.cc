#include "tictoctoe.h"
#include "allStateTable.h"
#include "winLoseTable.h"
#include <fstream>

void usage()
{
  std::cerr << "Usage: checkcsa csafile" << std::endl;
}

int main(int ac,char **ag)
{
  State s;
  AllStateTable allS("allstates.dat");     
  WinLoseTable winLose(allS,"winLoss.dat","winLossCount.dat");
  winLose.showSequence(s);
}
