#include "StudentWorld.h"
#include "GameConstants.h"


using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    chanceFungus = 0;
    chanceGoodie = 0;
    player = NULL;
    dirt = NULL;
}

StudentWorld::~StudentWorld(){cleanUp();}


bool StudentWorld::determineOverlap(int x, int y, Actor*& target){
    vector<Actor*>::iterator t = gameActors.begin();
    Actor* temp;
    while(t != gameActors.end()){
        temp = *t;
        if(calcDist(x, y, temp->getX(), temp->getY()) <= SPRITE_WIDTH){
            target = temp;
            return true;
        }
        t++;
    }
    return false;
}

void StudentWorld::generateDirtCoords(int& x, int&y){
    do{
        x = randInt(20, 236);
        y = randInt(20, 236);
    } while(sqrt( (128-x)*(128-x) + (128-y)*(128-y) ) > 120);
}

void StudentWorld::removeDeadObjects(){
    vector<Actor*>::iterator t = gameActors.begin();
    Actor* temp = NULL;
    while(t != gameActors.end()){
        temp = *t;
        if (!temp->isAlive()) {
            delete temp;
            gameActors.erase(t);
        } else
            t++;
    }
}

void StudentWorld::spawnFungus(){
    Actor* fungus = new Fungus(IID_FUNGUS, VIEW_RADIUS, VIEW_RADIUS, 0, true, this,  max(randInt(0, 300 - 10 * getLevel() - 1), 50), 1);
    spawnItem(fungus);
}

void StudentWorld::spawnGoodie(){
    int whichGoodie = randInt(1, 10);
    Actor* goodie;
    if(whichGoodie == 1)
        goodie = new ExtraLife(IID_EXTRA_LIFE_GOODIE, VIEW_RADIUS, VIEW_RADIUS, 0, true, this,  max(randInt(0, 300 - 10 * getLevel() - 1), 50), 1);
    else if(whichGoodie <= 4) //its not 1 so 2,3,4
        goodie = new FlamethrowerItem(IID_FLAME_THROWER_GOODIE, VIEW_RADIUS, VIEW_RADIUS, 0, true, this, max(randInt(0, 300 - 10 * getLevel() - 1), 50), 1);
    else goodie = new RestoreHealth(IID_RESTORE_HEALTH_GOODIE, VIEW_RADIUS, VIEW_RADIUS, 0, true, this, max(randInt(0, 300 - 10 * getLevel() - 1), 50), 1);
    spawnItem(goodie);
}

void StudentWorld::addNewActors(){
    //decide what to spawn if anything;
    chanceFungus = max(510 - getLevel() * 10, 200);
    chanceGoodie = max(510 - getLevel() * 10, 250);
    int fungusIfZero = randInt(0, chanceFungus);
    int goodieIfZero = randInt(0, chanceGoodie);
    if (fungusIfZero == 0) {
        spawnFungus();
    }
    if (goodieIfZero == 0) {
        spawnGoodie();
    }

}

void StudentWorld::updateGameText(){
    string text = "";
    ostringstream sout;//gather game's info
    sout << "Score: " << getScore() << "  " << "Level: " << getLevel() << "  " << "Lives: " << getLives() << "  "
        << "Health: " << player->getHp() << "  " << "Sprays: " << player->getSprays() << "  " << "Flames: " << player->getFlames() << "  ";
    text = sout.str();
    setGameStatText(text);
}

int StudentWorld::init()
{
    int rndX = 0, rndY = 0;
    player = new Socrates(IID_PLAYER, 0, VIEW_HEIGHT/2, true, this, true, 0, true, 180, 100, 5, 20);
    gameActors.push_back(player);
    for (int i = 0; i < max(180 - 20 * getLevel(), 20);
         i++) {
        generateDirtCoords(rndX, rndY);
        dirt = new Dirt(IID_DIRT, rndX, rndY, true, this, true, 0);
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
    removeDeadObjects();
    addNewActors(); //hasnt been implemented
    updateGameText();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(int i = 0; i < gameActors.size(); i++){
        delete gameActors[i];
    }
}
