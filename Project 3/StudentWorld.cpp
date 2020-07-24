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
    m_numPits = getLevel();
    chanceFungus = 0;
    chanceGoodie = 0;
    player = NULL;
    dirt = NULL;
}

StudentWorld::~StudentWorld(){cleanUp();}


bool StudentWorld::determineOverlap(int x, int y, Actor*& target){
    for(int i = 0; i < gameActors.size(); i++){
        if(calcDist(x, y, gameActors[i]->getX(), gameActors[i]->getY()) <= SPRITE_WIDTH){
            target = gameActors[i];
            return true;
        }
    }
    return false;
}

bool StudentWorld::determineBlockingOverlap(int x, int y, Actor*& target){
    for(int i = 0; i < gameActors.size(); i++){
        if(gameActors[i]->blocksStuff() && calcCollided(x, y, gameActors[i]->getX(), gameActors[i]->getY())){
            target = gameActors[i];
            return true;
        }
    }
    return false;
}

void StudentWorld::generateRandCoords(int& x, int&y){
    Actor* temp;
    do{
        x = randInt(20, 236);
        y = randInt(20, 236);
    } while(calcDist(x, y, VIEW_RADIUS, VIEW_RADIUS) > 120 || determineOverlap(x, y, temp));
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
    sout.fill('0');
    sout << "Score: " << setw(6) << getScore();
    sout << "  " << "Level: " << getLevel() << "  " << "Lives: " << getLives() << "  "
        << "Health: " << player->getHp() << "  " << "Sprays: " << player->getSprays() << "  " << "Flames: " << player->getFlames() << "  ";
    text = sout.str();
    setGameStatText(text);
}

int StudentWorld::init()
{
    m_numPits = getLevel();
    int rndX = 0, rndY = 0;
    player = new Socrates(this);
    gameActors.push_back(player);
    for (int i = 0; i < getLevel(); i++) {
        generateRandCoords(rndX, rndY);
        pit = new Pit(IID_PIT, rndX, rndY, this, 5, 3, 2);
        gameActors.push_back(pit);
    }
    for (int i = 0; i <  min(5 * getLevel(), 25); i++) {
        generateRandCoords(rndX, rndY);
        food = new Food(IID_FOOD, rndX, rndY, true, this);
        gameActors.push_back(food);
    }
    for (int i = 0; i < max(180 - 20 * getLevel(), 20);
         i++) {
        generateRandCoords(rndX, rndY);
        dirt = new Dirt(IID_DIRT, rndX, rndY, true, this, true, 0);
        gameActors.push_back(dirt);
    }
    return GWSTATUS_CONTINUE_GAME;
}

Actor* StudentWorld::closestFood(vector<Actor*> foods, Actor* me){
    if(foods.size() == 0){return nullptr;}
    if(foods.size() == 1){return foods[0];}
    Actor* closest = nullptr;
    int dist1;
    int dist2;
    for(int i = 0; i < foods.size()-1; i+= 1){
        dist1 = calcDist(foods[i], me);
        dist2 = calcDist(foods[i+1], me);
        if(dist1 <= dist2)
            closest = foods[i];
    }
    return closest;
}

bool StudentWorld::foodNearMe(Actor* me, Actor* &food){
    double X = me->getX();
    double Y = me->getY();
    Actor* temp;
    vector<Actor*> foods;
    for (int i = 0; i < gameActors.size(); i++) {
        temp = gameActors[i];
        if (temp->isFood() && calcDist(X, Y, temp->getX(), temp->getY()) < VIEW_RADIUS) {
            foods.push_back(temp);
        }
    }
    food = closestFood(foods, me);
    if(food != nullptr)return true;
    return false;
}

bool StudentWorld::levelComplete(){
    m_levelComplete = true;
    for(int i = 0; i < gameActors.size(); i++){
        if(gameActors[i]->isEnemy())
            m_levelComplete = false;
    }
    return m_levelComplete;
}

int StudentWorld::move()
{
    for(int i = 0; i < gameActors.size(); i++){
        if(gameActors[i]->isAlive()){
            gameActors[i]->doSomething();
            if(!player->isAlive()){
                decLives();
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            } if(levelComplete() && m_numPits == 0){
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    removeDeadObjects();
    addNewActors();
    updateGameText();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while(!gameActors.empty()) {
        delete gameActors.back();
        gameActors.pop_back();
    }
}
