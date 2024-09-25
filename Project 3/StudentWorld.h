#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include "Actor.h"
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  bool isSpaceAvailable(int x, int y);
  bool addPea(int x, int y, int direction, int source);
  void addRobot(int x, int y, int type);
  bool damageAll(int x, int y, Actor* acting);
  Actor* findActor(int x, int y);
  Actor* findActor(int x, int y, Actor* caller);
  bool canSwallowAt(int x , int y);
  void pickUp();
  bool isPlayerHere(int x , int y);
  void addPeas(int amount){m_player->addPeas(amount);}
  bool areOtherTheifsHere(int x, int y);
  //int getScore(){return score;}
  //int getLevel(){return level;}
  //void nextLevel(){level+=1;}
  //int getLives(){return lives;}
  //void upLives(int x ){lives+=x;}
  void restorePlayer(){m_player->damage(-1*(20-m_player->getHealth()));}
  int getBonus(){return bonus;}
  bool findSight(int x, int y, int dx,int dy);
  void setFinished(){finished=1;}
  int countWorkers(int x, int y);
  Actor * findConsumableOn(int x, int y);
private:
  Player *m_player;
  std::vector<Actor*> actors; // for all the things
  int m_nCrystals;
  //int score=0;
  //int level=0;
  //int lives=3;
  int bonus;
  int finished;
};

#endif // STUDENTWORLD_H_
