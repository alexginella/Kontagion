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

private:
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
    inline double calcDist(int x1, int y1, int x2, int y2){return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );}
    void generateDirtCoords(int& x, int& y);
    Socrates* player;
    Actor* dirt;
    vector<Actor*> gameActors;
};

#endif // STUDENTWORLD_H_
