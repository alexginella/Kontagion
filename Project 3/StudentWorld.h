#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath); //and destructor
    ~StudentWorld();
    void removeDeadObjects();
    void addNewActors();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    bool determineOverlap(int x, int y, Actor*& target);
    
    void updateGameText();
    
    inline void addActor(Actor* actor){gameActors.push_back(actor);}
    
    inline void pitDepleted(){m_numPits--;}
    
    inline double calcDist(int x1, int y1, int x2, int y2){return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );}
    
    inline double calcDist(Actor* a1, Actor* a2){return sqrt( (a1->getX()-a2->getX())*(a1->getX()-a2->getX()) + (a1->getY()-a2->getY())*(a1->getY()-a2->getY()) );}
    
    inline bool calcCollided(int x1, int y1, int x2, int y2){return calcDist(x1, y1, x2, y2) <= SPRITE_WIDTH/2;}
    
    bool foodNearMe(Actor* me, Actor* &food);
    
    bool determineBlockingOverlap(int x, int y, Actor*& target);
        
    inline double calcDistFromPlayer(int x, int y){return calcDist(x, y, player->getX(), player->getY());}
    
    inline void damagePlayer(int amr){player->takeDamage(amr);}
    
    inline void getPlayerCoords(int& x, int& y){x = player->getX(); y = player->getY();}
    
    inline void increaseFlames(int amt){player->increaseFlames(amt);}
    
    bool levelComplete();
    
private:
    Actor* closestFood(vector<Actor*> foods, Actor* me);
    int m_numPits;
    bool m_levelComplete;
    int chanceFungus;
    int chanceGoodie;
    void spawnGoodie();
    void spawnFungus();
    inline int max(int n1, int n2){
        if(n1 >= n2) return n1;
        else return n2;
    }
    inline void spawnItem(Actor* item){
        int angle = randInt(0, 359);
        item->moveAngle(angle, VIEW_RADIUS);
        gameActors.push_back(item);
    }
    void generateRandCoords(int& x, int& y);
    Socrates* player;
    Actor* dirt;
    Actor* food;
    Actor* pit;
    vector<Actor*> gameActors;
};

#endif // STUDENTWORLD_H_
