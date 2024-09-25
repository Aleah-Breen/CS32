#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject{
    public: 
        Actor(StudentWorld *world, int imageId, double startX, double startY, int startDirection, double size);
        virtual void doSomething() = 0;
        virtual void damage(int amount){return;};
        StudentWorld* getWorld();
        bool isAlive(){return alive;};
        void setAlive(bool newStatus){alive = newStatus;};
        virtual bool isPushable(){return false;};
        virtual bool push(int x, int y, int dir){return false;};
        virtual bool canSwallow(){return false;};
        virtual bool isPassable(){return false;};
        virtual bool canBeStepped(){return false;};
        virtual void consume(){return;};
        virtual bool fromFactory(){return false;}
        virtual bool isConsumable(){return false;}
        virtual int getType(){return -1;};
        virtual bool canTheifsWalk(){return false;}
        virtual bool hasStolenItem(){return false;}
        virtual int getGoodie(){return 0;}
        virtual bool canSteal(){return false;}


    private:
        StudentWorld* m_world;
        bool alive;

};
class Consumable : public Actor{
    public:
        Consumable(StudentWorld * world,int imageId, double x, double y);
        virtual void doSomething() = 0;
        virtual bool canBeStepped(){return true;};
        virtual bool isPassable(){return true;};
        virtual void consume();
        virtual bool isConsumable(){return true;}
        virtual bool canTheifsWalk();
        virtual void distributePoints()=0;


};
class Crystal : public Consumable{
    public:    
        Crystal(StudentWorld * world, double x, double y);
        virtual void doSomething();
        virtual bool isConsumable(){return false;}
        virtual void distributePoints();

};
class Goodie : public Consumable{
    public:
        Goodie(StudentWorld * world,int imageId, double x, double y);
        virtual void doSomething();
        virtual void doActivity()=0;
};
class Ammo : public Goodie{
    public:
        Ammo(StudentWorld * world, double x, double y);
        virtual void doActivity();
        virtual int getType(){return 1;}
        virtual void distributePoints();

};
class ExtraLife : public Goodie{
    public:
        ExtraLife(StudentWorld * world, double x , double y);
        virtual void doActivity();
        virtual int getType(){return 2;}
        virtual void distributePoints();
};
class Restore : public Goodie{
    public:
        Restore(StudentWorld * world, double x, double y);
        virtual void doActivity();
        virtual int getType(){return 3;}
        virtual void distributePoints();
};
class Pit : public Actor{
    public:
        Pit(StudentWorld * world, double x, double y);
        virtual void doSomething();
        virtual bool canSwallow(){return true;};
        virtual bool isPassable(){return true;};
};
class livingActor : public Actor{
    public:
        livingActor(StudentWorld *world, int imageId, double x, double y, int direction, double size, int health);
        virtual void doSomething() = 0;
        int getHealth(){return m_health;};
        virtual void damage(int amount);
        virtual bool push(int x, int y, int dir){return false;};
        virtual void playUniqueSound()=0;
        virtual void playDeathSound()=0;
        virtual void giveDeathPoints()=0;
    private:
        int m_health;
};
class Wall : public Actor{
    public:
        Wall(StudentWorld *world,double x,double y);
        virtual void doSomething(){return;};
    private:
    // nothing??
};
class Marble : public livingActor{
    public:
        Marble(StudentWorld *world, double x, double y);
        virtual void doSomething(){return;};
        virtual bool isPushable(){return true;};
        virtual bool push(int x, int y, int dir);
        virtual void playUniqueSound(){return;}
        virtual void playDeathSound(){return;}
        virtual void giveDeathPoints(){return;}
    private:
};
class Pea : public Actor{
    public:
        Pea(StudentWorld *world, double x, double y, int direction);
        virtual void doSomething();
        virtual bool canBeStepped(){return true;}
    private:
};
class Player : public livingActor{
    public:
        Player(StudentWorld *world, double x, double y);
        virtual void doSomething();
        int getPeas(){return m_peas;};
        void usePea(){m_peas-=1;};
        void addPeas(int amount){m_peas+=amount;}
        virtual void playUniqueSound();
        virtual void playDeathSound();
        virtual void giveDeathPoints(){return;}

    private:
        int m_peas;
};
class Robot : public livingActor{
    public:
        Robot(StudentWorld *world, int imageId, double x, double y, int direction, int health);
        virtual void doSomething();
        virtual void doRobotActivity()=0;
        virtual void playUniqueSound();
        virtual void playDeathSound();
    private:  
        int m_ticks;
        int currentTick;
};
class RageBot : public Robot{
    public:
        RageBot(StudentWorld * world, double x, double y, int direction);
        virtual void doRobotActivity();
        virtual void giveDeathPoints();

    private:
        int deltaX;
        int deltaY;
};
class ThiefBot : public Robot{
    public:
        ThiefBot(StudentWorld * world, double x, double y);
        virtual void doRobotActivity();
        virtual bool hasStolenItem();
        virtual int getGoodie(){return goodie;}
        virtual bool targetFound()=0;
        virtual bool fromFactory(){return true;}
        virtual bool canSteal(){return true;}
        virtual int getDeltaX(){return deltaX;}
        virtual int getDeltaY(){return deltaY;}        
    private:
        int distBeforeTurning;
        int distSoFar;
        int goodie; // 0 = none, 1 = ammo, 2 = extra life, 3 = restore
        int deltaX;
        int deltaY;
};
class RegularThiefBot : public ThiefBot{
    public:
        RegularThiefBot(StudentWorld * world, double x, double y);
        virtual bool targetFound(){return false;}
        virtual void giveDeathPoints();

};
class MeanThiefBot : public ThiefBot{
    public:
        MeanThiefBot(StudentWorld * world, double x, double y);
        virtual bool targetFound();
        virtual void giveDeathPoints();

};
class ThiefBotFactory : public Actor{
    public:
        ThiefBotFactory(StudentWorld * world, double x ,double y, int type);
        virtual void doSomething();
        int getType(){return m_type;}
    private:
        int m_type; // 0 = regular theifbot, 1 = mean theif bot.
};
class Exit : public Actor{
    public:
        Exit(StudentWorld *world, double x, double y);
        virtual void doSomething();
        virtual bool canBeStepped(){return true;};
        virtual bool isPassable(){return true;};
};
#endif // ACTOR_H_
