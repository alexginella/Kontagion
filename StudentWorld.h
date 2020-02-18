#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath); //and destructor
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

private:
    inline int max(int n1, int n2){
        if(n1 >= n2) return n1;
        else return n2;
    }
    void generateRandCoords(int& x, int& y);
    Actor* player;
    Actor* dirt;
    vector<Actor*> gameActors;
};

#endif // STUDENTWORLD_H_
