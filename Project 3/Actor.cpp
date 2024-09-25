#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor stuff
Actor::Actor(StudentWorld *world, int imageId, double startX, double startY, int startDirection, double size)
:
GraphObject(imageId, startX, startY, startDirection, size),
m_world(world)
{setVisible(true);
    alive=true;
};
StudentWorld* Actor::getWorld(){return m_world;};

// Wall stuff
Wall::Wall(StudentWorld *world,double x,double y):
    Actor(world, IID_WALL, x, y, right, 1)
    {
    };
// living actor stuff
livingActor::livingActor(StudentWorld *world, int imageId, double x, double y, int direction, double size, int health):
    Actor(world,imageId,x,y,direction,size)
{m_health=health;}
void livingActor::damage(int amount){
    
    m_health-=amount;
    if(m_health<=0){
        playDeathSound();
        setAlive(false);
        giveDeathPoints();
    }
    else{
        playUniqueSound();
    }
}
Consumable::Consumable(StudentWorld * world, int imageId, double x, double y)
:   Actor(world, imageId, x , y, right, 1) 
{}
void Consumable::consume(){
    setAlive(false);
}
bool Consumable::canTheifsWalk(){
    if(getWorld()->areOtherTheifsHere(getX(),getY())){
        return false;
    }
    return true;
}
Crystal::Crystal(StudentWorld * world, double x, double y)
:   Consumable(world, IID_CRYSTAL, x , y)
{}
void Crystal::distributePoints(){
    getWorld()->increaseScore(50);
}
void Crystal::doSomething(){
    if(getWorld()->isPlayerHere(getX(), getY())){
        getWorld()->pickUp();
        setAlive(false);
        distributePoints();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}
Pit::Pit(StudentWorld * world, double x, double y):
    Actor(world, IID_PIT, x, y, right, 1)    
{}
void Pit::doSomething(){
    Actor * a = getWorld()->findActor(getX(), getY(),this);
    if(a!=nullptr && a->isPushable()){
        a->setAlive(false);
        setAlive(false);
        return;
    }
}
Marble::Marble(StudentWorld *world,double x,double y):
    livingActor(world, IID_MARBLE, x, y, right, 1,10)
    {
    };
bool Marble::push(int x, int y, int dir){
    switch(dir){
        case right:
            if(getWorld()->isSpaceAvailable(x+1,y)||getWorld()->canSwallowAt(x+1,y)){
                return true;
            }
            break;
        case up:
            if(getWorld()->isSpaceAvailable(x,y+1)||getWorld()->canSwallowAt(x,y+1)){
                return true;
            }
            break;
        case down:
            if(getWorld()->isSpaceAvailable(x,y-1)||getWorld()->canSwallowAt(x,y-1)){
                return true;
            }
            break;
        case left:
            if(getWorld()->isSpaceAvailable(x-1,y)||getWorld()->canSwallowAt(x-1,y)){
                return true;
            }
            break;
        default:
            return false;
            break;
    }
    return false;
}
// Avatar stuff
Player::Player(StudentWorld *world, double x, double y):
    livingActor(world, IID_PLAYER, x,y,right,1,20)  
{m_peas=20;}
void Player::playUniqueSound(){
    getWorld()->playSound(SOUND_PLAYER_IMPACT);
}
void Player::playDeathSound(){
    getWorld()->playSound(SOUND_PLAYER_DIE);
}
void Player::doSomething(){
    if(getHealth()<=0){
        return;
    }
    
    int character;
    // if the user clicked an arrow in this frame this if statement returns true
    // effective since we dont want to do anything if the player does not click a key
    // stores it in character
    if(getWorld()->getKey(character)){
        if(character==KEY_PRESS_ESCAPE){
            damage(100); // 100 ensures the player has a negative health
        }
        else if(character==KEY_PRESS_SPACE){
            if(getPeas()>0){
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                switch(getDirection()){
                    case up:
                        getWorld()->addPea(getX(),getY()+1,up,0);
                        break;
                    case down:
                        getWorld()->addPea(getX(),getY()-1,down,0);
                        break;
                    case left:
                        getWorld()->addPea(getX()-1,getY(),left,0);
                        break;
                    case right:
                        getWorld()->addPea(getX()+1,getY(),right,0);
                        break;
                }
            }
        }
        else if(character==KEY_PRESS_LEFT){
            if(getWorld()->isSpaceAvailable(getX()-1,getY())){
                moveTo(getX()-1,getY());
            }
            else{
                Actor * a = getWorld()->findActor(getX()-1,getY());
                if(a!=nullptr){   
                    if(a->isPushable()){
                        if(a->push(getX()-1,getY(),left)){
                            moveTo(getX()-1,getY());
                            a->moveTo(getX()-1,getY());
                        }
                    }
                    else if(a->canBeStepped()){
                        moveTo(getX()-1,getY());
                        a->consume();
                    }
                } 
            }    
            setDirection(left);    
        }
        else if(character==KEY_PRESS_RIGHT){
            if(getWorld()->isSpaceAvailable(getX()+1,getY())){
                moveTo(getX()+1,getY());
            }
            else{
                Actor * a = getWorld()->findActor(getX()+1,getY());
                if(a!=nullptr){   
                    if(a->isPushable()){
                        if(a->push(getX()+1,getY(),right)){
                            moveTo(getX()+1,getY());
                            a->moveTo(getX()+1,getY());
                        }
                    }
                    else if(a->canBeStepped()){
                        moveTo(getX()+1,getY());
                        a->consume();
                    }
                } 
            }    
            setDirection(right);
        }
        else if(character==KEY_PRESS_DOWN){
            if(getWorld()->isSpaceAvailable(getX(),getY()-1)){
                moveTo(getX(),getY()-1);
            }
            else{
                Actor * a = getWorld()->findActor(getX(),getY()-1);
                if(a!=nullptr){   
                    if(a->isPushable()){
                        if(a->push(getX(),getY()-1,down)){
                            moveTo(getX(),getY()-1);
                            a->moveTo(getX(),getY()-1);
                        }
                    }
                    else if(a->canBeStepped()){
                        moveTo(getX(),getY()-1);
                        a->consume();
                    }
                } 
            }    
            setDirection(down);
        }
        else if(character==KEY_PRESS_UP){
            if(getWorld()->isSpaceAvailable(getX(),getY()+1)){
                moveTo(getX(),getY()+1);
            }
            else{
                Actor * a = getWorld()->findActor(getX(),getY()+1);
                if(a!=nullptr){   
                    if(a->isPushable()){
                        if(a->push(getX(),getY()+1,up)){
                            moveTo(getX(),getY()+1);
                            a->moveTo(getX(),getY()+1);
                        }
                    }
                    else if(a->canBeStepped()){
                        moveTo(getX(),getY()+1);
                        a->consume();
                    }
                } 
            }    
            setDirection(up);
        }
    } 
}

//pea stuff
Pea::Pea(StudentWorld *world, double x, double y, int direction):
    Actor(world,IID_PEA,x,y,direction,1)
{
}
void Pea::doSomething(){
    if(isAlive()){
        if(getWorld()->damageAll(getX(),getY(),this)){
            setVisible(false);
            setAlive(false);
        }
    }
    switch(getDirection()){
        case up:         
            moveTo(getX(),getY()+1);
        break;
        case down:    
            moveTo(getX(),getY()-1);  
        break;
        case left:
            moveTo(getX()-1,getY());
        break;
        case right:
            moveTo(getX()+1,getY());
        break;
    }
    if(isAlive()){
        if(getWorld()->damageAll(getX(),getY(),this)){
            setVisible(false);
            setAlive(false);
        }
    }
}

// rage bot stuff
Robot::Robot(StudentWorld* world,int imageId, double x, double y, int direction, int health):
    livingActor(world,imageId, x,y,direction,1,health)
{
    
    currentTick=1;
    m_ticks = (28-getWorld()->getLevel())/4;
    if(m_ticks<3){
        m_ticks =3;
    }
};
void Robot::playUniqueSound(){
    getWorld()->playSound(SOUND_ROBOT_IMPACT);
}
void Robot::playDeathSound(){
    getWorld()->playSound(SOUND_ROBOT_DIE);
}
void Robot::doSomething(){
    if(currentTick==m_ticks){
        doRobotActivity();
        currentTick=1;
    }
    else{
        currentTick+=1;
    }
};
RageBot::RageBot(StudentWorld * world, double x, double y, int direction):
    Robot(world, IID_RAGEBOT, x, y, direction,10)
{
    if(direction==right){
        deltaX=1;
        deltaY=0;
    }
    else{
        deltaY=1;
        deltaX=0;
    }
};
void RageBot::giveDeathPoints(){
    getWorld()->increaseScore(100);
}
void RageBot::doRobotActivity(){
    bool lineOfSight = getWorld()->findSight(getX()+deltaX,getY()+deltaY,deltaX,deltaY); //later set lineOfSight = findSight();
    if(lineOfSight){
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        getWorld()->addPea(getX()+deltaX,getY()+deltaY,getDirection(),1);
    }
    else{
        if(getWorld()->isSpaceAvailable(getX()+deltaX,getY()+deltaY) || (getWorld()->findActor(getX()+deltaX,getY()+deltaY)->canBeStepped())){
            moveTo(getX()+deltaX,getY()+deltaY);
        }
        else{
            switch(getDirection()){
                case up:
                    deltaY=-1;
                    setDirection(down);
                break;
                case down:
                    deltaY=1;
                    setDirection(up);
                break;
                case left:
                    deltaX=1;
                    setDirection(right);
                break;
                case right:
                    deltaX=-1;
                    setDirection(left);
                break;
            }
        }
    }
}
ThiefBot::ThiefBot(StudentWorld * world, double x,  double y):
    Robot(world, IID_THIEFBOT, x, y, right,5), goodie(0) , distSoFar(0), deltaX(1), deltaY(0)
{
    distBeforeTurning= randInt(1,6);
}

bool ThiefBot::hasStolenItem(){
    
    if(goodie==0){
        return false;
    }
    return true;
}
void ThiefBot::doRobotActivity(){
    if(targetFound()){
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }
    Actor * a = getWorld()->findConsumableOn(getX(), getY());
    if(goodie!=0){
        a=nullptr;
    }
    if(a!=nullptr){
        if(!a->isConsumable()){
            a=nullptr;
        }
    }
    if(a!=nullptr && randInt(1,10==1)){
            goodie = a->getType();
            a->consume();
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
    }
    else if(distSoFar<distBeforeTurning){
        a = getWorld()->findActor(getX()+deltaX,getY()+deltaY);
        if(a!=nullptr){
            if(a->canTheifsWalk()){
                if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                    moveTo(getX()+deltaX,getY()+deltaY);
                    distSoFar++;
                }
            }
            else{
                distBeforeTurning=randInt(1,10);
                int direction = randInt(0,3);
                for(int j=0;j<4;j++){
                    switch((direction+j)%4){
                        case 0: // left
                            deltaX=-1;
                            deltaY=0;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(left);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                }
                                break;
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){

                                setDirection(left);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                            break;
                        case 1: // right
                            deltaX=1;
                            deltaY=0;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(right);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                }
                                break;
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                setDirection(right);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                            break;
                        case 2: // up 
                            deltaX=0;
                            deltaY=1;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(up);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                }
                                break;
                            }
                           else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                setDirection(up);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                            break;
                        case 3: // down
                            deltaX=0;
                            deltaY=-1;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(down);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                break;
                                }
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                setDirection(down);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                        break;
                
                    }
                }
                switch(direction){
                        case 0: // left
                            deltaX=-1;
                            deltaY=0;
                            setDirection(left);
                            break;
                        case 1: // right
                            deltaX=1;
                            deltaY=0;
                            setDirection(right);
                            break;
                        case 2: // up 
                            deltaX=0;
                            deltaY=1;
                            setDirection(up);
                            break;
                        case 3: // down
                            deltaX=0;
                            deltaY=-1;
                            setDirection(down);
                        break;
                
                }
            }
        }  
        else{
            Actor * a = getWorld()->findActor(getX()+deltaX,getY()+deltaY);
            if(a==nullptr){
                // do nothing
            }
            else if(a->canTheifsWalk()){
                a=nullptr;
            }
            if(getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)||a!=nullptr){
                distBeforeTurning=randInt(1,10);
                int direction = randInt(0,3);
                for(int j=0;j<4;j++){
                    switch((direction+j)%4){
                        case 0: // left
                            deltaX=-1;
                            deltaY=0;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(left);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                }
                                break;
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){

                                setDirection(left);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                            break;
                        case 1: // right
                            deltaX=1;
                            deltaY=0;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(right);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                }
                                break;
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                setDirection(right);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                            break;
                        case 2: // up 
                            deltaX=0;
                            deltaY=1;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(up);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                }
                                break;
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                setDirection(up);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                            break;
                        case 3: // down
                            deltaX=0;
                            deltaY=-1;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(down);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                break;
                                }
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                setDirection(down);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                        break;
                
                    }
                }
                switch(direction){
                        case 0: // left
                            deltaX=-1;
                            deltaY=0;
                            setDirection(left);
                            break;
                        case 1: // right
                            deltaX=1;
                            deltaY=0;
                            setDirection(right);
                            break;
                        case 2: // up 
                            deltaX=0;
                            deltaY=1;
                            setDirection(up);
                            break;
                        case 3: // down
                            deltaX=0;
                            deltaY=-1;
                            setDirection(down);
                        break;
                
                }
            }
            else{
                moveTo(getX()+deltaX,getY()+deltaY);
                distSoFar++;
            }
        }
        
    }
    else{
                distBeforeTurning=randInt(1,10);
                int direction = randInt(0,3);
                for(int j=0;j<4;j++){
                    switch((direction+j)%4){
                        case 0: // left
                            deltaX=-1;
                            deltaY=0;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(left);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                }
                                break;
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){

                                setDirection(left);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                            break;
                        case 1: // right
                            deltaX=1;
                            deltaY=0;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(right);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                }
                                break;
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                setDirection(right);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                            break;
                        case 2: // up 
                            deltaX=0;
                            deltaY=1;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(up);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                }
                                break;
                            }
                           else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                setDirection(up);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                            break;
                        case 3: // down
                            deltaX=0;
                            deltaY=-1;
                            a=getWorld()->findActor(getX()+deltaX,getY()+deltaY);
                            if(a!=nullptr){    
                                if(a->canTheifsWalk()){
                                    if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                        setDirection(down);
                                        moveTo(getX()+deltaX,getY()+deltaY);
                                        return;
                                    }
                                break;
                                }
                            }
                            else if(!getWorld()->isPlayerHere(getX()+deltaX,getY()+deltaY)){
                                setDirection(down);
                                moveTo(getX()+deltaX,getY()+deltaY);
                                return;
                            }
                        break;
                
                    }
                }
                switch(direction){
                        case 0: // left
                            deltaX=-1;
                            deltaY=0;
                            setDirection(left);
                            break;
                        case 1: // right
                            deltaX=1;
                            deltaY=0;
                            setDirection(right);
                            break;
                        case 2: // up 
                            deltaX=0;
                            deltaY=1;
                            setDirection(up);
                            break;
                        case 3: // down
                            deltaX=0;
                            deltaY=-1;
                            setDirection(down);
                        break;
                
                }
            }
}
ThiefBotFactory::ThiefBotFactory(StudentWorld *world, double x, double y, int type):
    Actor(world, IID_ROBOT_FACTORY, x, y, 1, 1), m_type(type)
{}
void ThiefBotFactory::doSomething(){
    int numberOfWorkers=0;
    for(int i=-3;i<=3;i++){
        for(int j=-3; j<=3;j++){
            numberOfWorkers+=getWorld()->countWorkers(getX()+i,getY()+j);   
        }
    }
    if(numberOfWorkers<3){
        Actor * a = getWorld()->findActor(getX(),getY(),this);
        if(a==nullptr){
            if(randInt(1,50)==1){
                getWorld()->playSound(SOUND_ROBOT_BORN);
                getWorld()->addRobot(getX(),getY(),getType());
            }
        }
    }
}
Exit::Exit(StudentWorld* world, double x, double y):
    Actor(world, IID_EXIT, x, y, right, 1)
{
    setVisible(false);
};
void Exit::doSomething(){
    if(getWorld()->isPlayerHere(getX(),getY()) && isVisible()){
        getWorld()->setFinished();
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
    }
    return;
}
Goodie::Goodie(StudentWorld* world, int imageId, double x, double y):
    Consumable(world,imageId, x, y)
{}
void Goodie::doSomething(){
    if(getWorld()->isPlayerHere(getX(), getY())){
        //play sound
        setAlive(false);
        doActivity();
        distributePoints();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}
Ammo::Ammo(StudentWorld * world, double x, double y):
    Goodie(world, IID_AMMO, x, y)
{}
void Ammo::doActivity(){
    getWorld()->addPeas(20);
}
void Ammo::distributePoints(){
    getWorld()->increaseScore(100);
}
ExtraLife::ExtraLife(StudentWorld* world, double x, double y):
    Goodie(world, IID_EXTRA_LIFE, x, y)
{}
void ExtraLife::doActivity(){
    getWorld()->incLives();
}
void ExtraLife::distributePoints(){
    getWorld()->increaseScore(1000);
}
Restore::Restore(StudentWorld * world, double x, double y):
    Goodie(world, IID_RESTORE_HEALTH, x, y)
{}
void Restore::distributePoints(){
    getWorld()->increaseScore(500);
}
void Restore::doActivity(){
    getWorld()->restorePlayer();
}
RegularThiefBot::RegularThiefBot(StudentWorld * world, double x, double y):
    ThiefBot(world, x, y)
{}
void RegularThiefBot::giveDeathPoints(){
    getWorld()->increaseScore(10);
}
MeanThiefBot::MeanThiefBot(StudentWorld * world, double x, double y):
    ThiefBot(world,x,y)
{}
void MeanThiefBot::giveDeathPoints(){
    getWorld()->increaseScore(20);
}
bool MeanThiefBot::targetFound(){
    bool lineOfSight = getWorld()->findSight(getX()+getDeltaX(),getY()+getDeltaX(),getDeltaX(),getDeltaY()); //later set lineOfSight = findSight();
    if(lineOfSight){
        getWorld()->addPea(getX()+getDeltaX(),getY()+getDeltaY(),getDirection(),1);
        return true;
    }
    return false;
}
