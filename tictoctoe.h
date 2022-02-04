#ifndef _TICTOCTOE_H
#define _TICTOCTOE_H
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <complex>
#include <deque>
#include <string>
using namespace std;
typedef complex<int> Point;

typedef unsigned long long uint64;
typedef vector<uint64> vUint64;
typedef vector<char> vChar;
typedef unsigned char uchar;
typedef vector<uchar> vuChar;
typedef vector<short> vShort;
typedef vector<int> vInt;
//黒が先手、白が後手
enum Player{
  BLACK=1, WHITE=-1
};
//盤面のマスの状態
struct Ptype{
  enum {
    EMPTY=0,
    BLACK=1,
    WHITE=-1,
  };
};

struct State{
    char board[3*3];//局面情報
    int stands[2];//手持ちの情報
    int turn;//手番情報
    State(){
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++)
      board[x*3+y]=Ptype::EMPTY;
    stands[0]=5;
    stands[1]=4;
    turn=BLACK;
  }
  //９桁表記＋2桁の手持の情報表記
  uint64 show() const{
    uint64 a=0;
    uint64 h=1;
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++){
        if(board[x*3+y]!=-1){
          a+=board[x*3+y]*h;
        }else{
          a+=2*h;
        }
        h*=10;
      }
      a+=stands[0]*h;
      a+=stands[1]*h*10;
      return a;
  }
  //show()で表示された11桁表記からStateを生成
  State(uint64 p,bool test)
  {
    State s;
    uint64 a=p;
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++){
        char b=a%10;
        if(b==2){s.board[x*3+y]=-1;}else{s.board[x*3+y]=b;}
        a=a/10;
      }
    s.stands[0]=a%10;
    s.stands[1]=a/10%10;
    s.turn=BLACK;
    *this=s;
  }
  //正規化された局面情報からStateを生成
  State(uint64 p,Player pl=BLACK)
  {
    if(pl==BLACK) *this=makeBlackFromUint64(p);
    else *this=makeBlackFromUint64(p).rotateChangeTurn();
  }
  static State makeBlackFromUint64(uint64 p){
    State s;
    int i=0;
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++){
	char c=(p>>(i*2))&3;
	if((c&2)!=0) c-=4;
	s.board[x*3+y]=c;
	i++;
      }
    s.stands[0]=(p>>(18))&7;
    s.stands[1]=(p>>(18+3))&7;
    s.turn=BLACK;
    return s;
  }
  //手番と局面情報を反転する
  State rotateChangeTurn() const
  {
    State ret;
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++)
	ret.board[x*3+y]= -board[x*3+y];
    
    ret.stands[0]=stands[1]; 
    ret.stands[1]=stands[0]; 
    ret.turn= -turn;
    return ret;
  }
  //手番を反転する
  void changeTurn()
  {
    turn = -turn;
  }
  //局面を左右反転する
  State flipHorizontal() const
  {
    State ret;
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++)
	ret.board[x*3+y]= board[x*3+(2-y)];
  ret.stands[0]=stands[0]; 
    ret.stands[1]=stands[1]; 
    return ret;
  }
  //局面を上下反転する
  State flipUpsideDown() const
  {
    State ret;
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++)
	ret.board[x*3+y]= board[(2-x)*3+y];
  ret.stands[0]=stands[0]; 
    ret.stands[1]=stands[1]; 
    return ret;
  }
  //局面を時計回りに回転する
  State clockwiseRotate() const
  {
    State ret;
    
	ret.board[0]= board[6];
    ret.board[1]= board[3];
    ret.board[2]= board[0];
    ret.board[3]= board[7];
    ret.board[4]= board[4];
    ret.board[5]= board[1];
    ret.board[6]= board[8];
    ret.board[7]= board[5];
    ret.board[8]= board[2];
    ret.stands[0]=stands[0]; 
    ret.stands[1]=stands[1]; 
    return ret;
  }
  //局面情報を１と０のbit表現で生成
  uint64 pack() const
  {
    assert(turn==BLACK);
    uint64 ret=0ull;
    int i=0;
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++){
	ret|=static_cast<uint64>(board[x*3+y]&3)<<(i*2);
	i++;
      }
    for(int j=0;j<2;j++)
      ret|=static_cast<uint64>(stands[j])<<(18+j*3);
    return ret;
  }
  //局面情報から同じとみなせる７つの局面をだして最も小さい数字にして生成
  uint64 normalize() const{
    if(turn==BLACK){
      uint64 u1=pack();
      State a = flipHorizontal();
      uint64 u2=a.pack();
      u1=std::min(u1,u2);
      u2=a.clockwiseRotate().pack();
      u1=std::min(u1,u2);

      a = flipUpsideDown();
      u2=a.pack();
      u1=std::min(u1,u2);
      u2=a.clockwiseRotate().pack();
      u1=std::min(u1,u2);

      a=clockwiseRotate();
      u2=a.pack();
      u1=std::min(u1,u2);
      a=a.clockwiseRotate();
      u2=a.pack();
      u1=std::min(u1,u2);
      a=a.clockwiseRotate();
      u2=a.pack();
      u1=std::min(u1,u2);
      return u1;
    }
    else{
      State news=rotateChangeTurn();
      uint64 u1=news.pack();
      State a = news.flipHorizontal();
      uint64 u2=a.pack();
      u1=std::min(u1,u2);
      u2=a.clockwiseRotate().pack();
      u1=std::min(u1,u2);

      a = news.flipUpsideDown();
      u2=a.pack();
      u1=std::min(u1,u2);
      u2=a.clockwiseRotate().pack();
      u1=std::min(u1,u2);

      a=news.clockwiseRotate();
      u2=a.pack();
      u1=std::min(u1,u2);
      a=a.clockwiseRotate();
      u2=a.pack();
      u1=std::min(u1,u2);
      a=a.clockwiseRotate();
      u2=a.pack();
      u1=std::min(u1,u2);
      return u1;
    }
  }
  //負けているか判定
  bool isLoseBlack() const{
    if(board[0]+board[1]+board[2]==-3||
      board[3]+board[4]+board[5]==-3||
      board[6]+board[7]+board[8]==-3||
      board[0]+board[3]+board[6]==-3||
      board[1]+board[4]+board[7]==-3||
      board[2]+board[5]+board[8]==-3||
      board[0]+board[4]+board[8]==-3||
      board[2]+board[4]+board[6]==-3
      )return true;
      return false;
  }
  //つぎの局面をすべて生成
  vUint64 nextNStates() const{
    vUint64 ret;
    if(stands[0]==0)return ret;
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++){
          if(board[x*3+y]==0){
              State s(*this);
              s.board[x*3+y]=Ptype::BLACK;
              s.stands[0]--;
              s.changeTurn();
              ret.push_back(s.normalize());
          }
      }
      return ret;
  }
  vUint64 nextShowStates() const{
    vUint64 ret;
    if(stands[0]==0)return ret;
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++){
          if(board[x*3+y]==0){
              State s(*this);
              s.board[x*3+y]=Ptype::BLACK;
              s.stands[0]--;
              s.changeTurn();
              ret.push_back(s.rotateChangeTurn().show());
          }
      }
      return ret;
  }
  void draw() const{
    int p=1;
    if(stands[0]==stands[1])p=-1;
    for(int x=2;x>=0;x--){
      std::cerr << "|";
      for(int y=0;y<3;y++){
        if(board[x*3+y]==0){
          std::cerr << " |";
        }else if(board[x*3+y]==p*-1){
          std::cerr << "x|";
        }else if(board[x*3+y]==p*1){
          std::cerr << "o|";
        }
      }
      std::cerr << std::endl;
    }
  }
};
#endif