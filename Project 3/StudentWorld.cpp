#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    finished =0;
    string curLevel = "level0" + to_string(getLevel()) + ".txt";
    //cerr << curLevel<<endl;
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(curLevel);
    if(result == Level::load_fail_file_not_found || getLevel()==100){
        return GWSTATUS_PLAYER_WON;
    }
    m_nCrystals=0;
    bonus = 1000;
    if (result == Level::load_fail_file_not_found ||result == Level:: load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR; 
        //cerr << "Successfully loaded level" << endl;
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 16; y++) {
				Level::MazeEntry ge = lev.getContentsOf(x, y);
				switch (ge)
				{
                    case Level::mean_thiefbot_factory:
                        actors.push_back(new ThiefBotFactory(this,x,y,1));
                        break;
                    case Level::thiefbot_factory:
                        actors.push_back(new ThiefBotFactory(this,x,y,0));
                        break;
                    case Level::restore_health:
                        actors.push_back(new Restore(this,x,y));
                        break;
                    case Level::extra_life:
                        actors.push_back(new ExtraLife(this,x,y));
                        break;
                    case Level::ammo:
                        actors.push_back(new Ammo(this,x,y));
                        break;
                    case Level::exit:
                        actors.push_back(new Exit(this,x,y));
                        break;
                    case Level::crystal:
                        m_nCrystals +=1;
                        actors.push_back(new Crystal(this,x,y));
                        break;
                    case Level::pit:
                        actors.push_back(new Pit(this,x,y));
                        break;
                    case Level::marble:
                        actors.push_back(new Marble(this,x,y));
                        break;
                    case Level::horiz_ragebot:
                        actors.push_back(new RageBot(this,x,y,0));
                        break;
                    case Level::vert_ragebot:
                        actors.push_back(new RageBot(this,x,y,270));
                        break;
                    case Level::player:
                        m_player = new Player(this, x,y);
                        break;
                    case Level::wall:
                        actors.push_back(new Wall(this, x, y));
                        break;
                    default:
                        break;
				}
			}
		}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if(finished ==1){
        //advanceToNextLevel();
        increaseScore(2000+getBonus());
        return GWSTATUS_FINISHED_LEVEL;
    }
    // This code is here merely to allow the game to build, run, and terminate after you type q
    ostringstream oss;
    if(bonus<=0){
        bonus = 0;
    }
    else{
        bonus-=1;
    }     
	oss.fill('0');
	oss << "Score: ";
	oss << setw(7) << getScore() << "  ";
    oss << "Level: ";
	oss << setw(2) << getLevel() << "  ";
    oss.fill(' ');

    oss << "Lives: ";
	oss << getLives() << "  ";
    oss << "Health: ";
	oss << setw(3)<< m_player->getHealth()*5 << "%  ";
    oss << "Ammo: ";
    oss << m_player->getPeas() << "  ";
    oss << "Bonus: ";
    oss << setw(4) << getBonus() << "  ";
    string statText = oss.str();
    setGameStatText(statText);
    if(!m_player->isAlive()){
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    m_player->doSomething();
    for(int actorI=0;actorI<actors.size();actorI++){
        actors[actorI]->doSomething();
    }
    std::vector<Actor*>:: iterator it = actors.begin();
    while(it!=actors.end()){
        if(!(*it)->isAlive()){
            if((*it)->hasStolenItem()){
                switch(((*it)->getGoodie())){
                    case 1:
                        actors.push_back(new Ammo(this, (*it)->getX(),(*it)->getY()));
                        break;
                    case 2:
                        actors.push_back(new ExtraLife(this, (*it)->getX(),(*it)->getY()));
                        break;
                    case 3:
                        actors.push_back(new Restore(this, (*it)->getX(),(*it)->getY()));
                        break;
                    default:
                        break;
                }
            }
            delete *it;
            actors.erase(it);
            it--;
        }
        it++;
    }
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(int actorI=0;actorI<actors.size();actorI++){
        delete actors[actorI];
    }
    actors.clear();
    delete m_player;
    m_player=nullptr;
}

StudentWorld::~StudentWorld(){
    // according to Spec destructor can just call clean up
    cleanUp();
}
bool StudentWorld::isSpaceAvailable(int x,int y){
    for(int i=0;i<actors.size();i++){
        if(actors[i]->getX()==x && actors[i]->getY()==y){
            return false;
        }
    }
    return true;
}
bool StudentWorld::addPea(int x,int y, int direction, int source){
    actors.push_back(new Pea(this,x,y,direction));
    if(source==0){
        m_player->usePea();
    }      
    return true;
    
}
bool StudentWorld::damageAll(int x, int y, Actor* acting){
    for(int i=0;i<actors.size();i++){
        if(actors[i]->getX()==x && actors[i]->getY()==y && actors[i]!=acting && !(actors[i]->isPassable())){
            actors[i]->damage(2);
            return true;
        }
    }
    if(x==m_player->getX()&&y==m_player->getY()){
        m_player->damage(2);
        return true;
    }
    return false;
}
Actor* StudentWorld::findActor(int x, int y){
    for(int i=0;i<actors.size();i++){
        if(actors[i]->getX()==x && actors[i]->getY()==y){
            return actors[i];
        }
    }
    return nullptr;
}
Actor* StudentWorld::findActor(int x, int y, Actor* self){
    for(int i=0;i<actors.size();i++){
        if(actors[i]->getX()==x && actors[i]->getY()==y && actors[i]!=self){
            return actors[i];
        }
    }
    return nullptr;
}
bool StudentWorld::canSwallowAt(int x, int y){
    for(int i=0;i<actors.size();i++){
        if(actors[i]->getX()==x && actors[i]->getY()==y && actors[i]->canSwallow()){
            return true;
        }
    }
    return false;
}
void StudentWorld::pickUp(){
    m_nCrystals-=1;
    if(m_nCrystals<=0){
        for(auto a: actors){
            a->setVisible(true);
            playSound(SOUND_REVEAL_EXIT);
        }
    }
}
bool StudentWorld::isPlayerHere(int x, int y){
    if(x==m_player->getX()&&y==m_player->getY()){
        return true;
    }
    return false;
}
bool StudentWorld::findSight(int x, int y, int dx, int dy){
    Actor * a = findActor(x,y);
    if(x==m_player->getX() && y== m_player->getY()){
        return true;
    }
    if(a==nullptr){
        return findSight(x+dx,y+dy,dx,dy);
    }
    if(!a->isPassable()){
        return false;
    }
    
    return findSight(x+dx, y+dy, dx, dy);
}
int StudentWorld::countWorkers(int x, int y){
    int count =0;
    for(auto a : actors){
        if(a->getX()==x&&a->getY()==y&&a->fromFactory()){
            count +=1;
        }
    }
    return count;
}
void StudentWorld::addRobot(int x , int y, int type){
    switch(type){
        case 0:
            actors.push_back(new RegularThiefBot(this,x,y));
            break;
        case 1:
            actors.push_back(new MeanThiefBot(this,x,y));
            break;
    }
}
Actor * StudentWorld::findConsumableOn(int x, int y){
    for(auto a : actors){
        if(a->getX()==x && a->getY()==y && a->isConsumable()){
            return a;
        }
    }
    return nullptr;
}
bool StudentWorld::areOtherTheifsHere(int x, int y){
    for(auto a : actors){
        if(a->getX()==x && a->getY()==y && a->canSteal()){
            return true;
        }   
    }
    return false;
}
