#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    player = NULL;
    dirt = NULL;
}

StudentWorld::~StudentWorld(){cleanUp();}

void StudentWorld::generateRandCoords(int& x, int&y){
    
}

int StudentWorld::init()
{
    int rndX = 0, rndY = 0;
    player = new Socrates(IID_PLAYER, 0, VIEW_HEIGHT/2, true, this, 0, 180, 100, 5, 20);
    gameActors.push_back(player);
    for (int i = 0; i < max(180 - 20 * getLevel(), 20);
        i++) {
        
        dirt = new Dirt(IID_DIRT, rndX+20, rndY+20, true, this, 0);
        gameActors.push_back(dirt);
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    vector<Actor*>::iterator t = gameActors.begin();
    Actor* temp;
    while (t != gameActors.end()) {
        temp = *t;
        if(temp->isAlive())
            temp->doSomething();
        t++;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(int i = 0; i < gameActors.size(); i++){
        delete gameActors[i];
    }
}
