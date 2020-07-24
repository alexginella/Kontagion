#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <cassert>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, bool alive, StudentWorld* world, bool damageable, Direction dir, bool isPlayer, int health, int depth, double size, bool blocksStuff, bool isFood, bool isEnemy):m_isPlayer(isPlayer), m_damageable(damageable),  m_hp(health),  m_world(world),  m_isAlive(alive), m_blocksStuff(blocksStuff), m_isFood(isFood), m_isEnemy(isEnemy), GraphObject(imageID, startX, startY, dir, depth){

}




bool Actor::takeDamage(int amount){ //returns true if object dead
    m_hp -= amount;
    if(m_isPlayer){
        getWorld()->playSound(SOUND_PLAYER_HURT);
    }
    if(m_hp <= 0) {
        m_isAlive = false;
        return true;
    }
    return false;
}




Socrates::Socrates(StudentWorld* world)//IID_PLAYER, 0, VIEW_HEIGHT/2, true, this, true, 0, true, 180, 100, 5, 20
    :Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, true, world, true, true, 180, 100), m_angle(180), m_flameCharges(5), m_sprayCharges(20)
{
    
}

void Socrates::doSomething(){
    //do someting in a tick
    if(!isAlive()){return;}
    int ch;
    double sprayX, sprayY, flameX, flameY;
    if (getWorld()->getKey(ch)) {
        switch (ch){
            case KEY_PRESS_LEFT:
                m_angle += 5;
                moveAngle(getDirection(), VIEW_RADIUS);
                moveAngle(m_angle, VIEW_RADIUS);
                setDirection(m_angle + 180);
            break;
            case KEY_PRESS_RIGHT:
                 m_angle -= 5;
                moveAngle(getDirection(), VIEW_RADIUS);
                moveAngle(m_angle, VIEW_RADIUS);
                setDirection(m_angle + 180);
            break;
            case KEY_PRESS_SPACE:
                if(m_sprayCharges >= 1){
                    getPositionInThisDirection(getDirection(), 2*SPRITE_RADIUS, sprayX, sprayY);
                    //cout << sprayX << " == " << getX() << " " << sprayY << " == " << getY() << endl;
                    Actor* spray = new Spray(IID_SPRAY, sprayX, sprayY, getDirection(), SPRAY_TRAVEL, true, getWorld(), false, 1);
                    getWorld()->addActor(spray);
                    m_sprayCharges--;
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
            break;
        case KEY_PRESS_ENTER:
                if(m_flameCharges > 0){
                    //Add 16 flame objects exactly SPRITE_WIDTH pixels away from himself, starting directly in front of Socrates and in 22 degree
                    Actor* flame;
                    int flameDirection = getDirection();
                    for(int i = 0; i < AMMOUNT_FLAMES; i++){
                        getPositionInThisDirection(flameDirection, SPRITE_WIDTH, flameX, flameY);
                        flame = new Flame(IID_FLAME, flameX, flameY, flameDirection, FLAME_TRAVEL, true, getWorld(), false, 1);
                        flameDirection += 22;
                        getWorld()->addActor(flame);
                    }
                    m_flameCharges--;
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
            break;
        }
    }
    if(m_sprayCharges < MAX_SPRAYS)
        m_sprayCharges++;
}







Dirt::Dirt(int imageID, double startX, double startY, bool alive, StudentWorld* world, bool damageable, Direction dir):Actor(imageID, startX, startY, alive, world, damageable, dir, false, 0, 1, 1.0, true){
    
}

void Dirt::doSomething(){
    //doesnt do anything
}









Food::Food(int imageID, double startX, double startY, bool alive, StudentWorld* world)
    :Actor(imageID, startX, startY, alive, world, false, 90, false, 0, 1, 1.0, false, true){
    
}

void Food::doSomething(){
    //does nothing
}







Pit::Pit(int imageID, double startX, double startY, StudentWorld* world, int numSalmonella, int numAgroSalm, int numEcoli):m_numAgroSalm(numAgroSalm), m_numSalmonella(numSalmonella), numEcoli(numEcoli), Actor(imageID, startX, startY, true, world, false, 90, false, 0, 1){
    
}

void Pit::doSomething(){
    if(!isAlive()){return;}
    int bacteriaCount = m_numSalmonella + m_numAgroSalm + numEcoli;
    if(bacteriaCount <= 0){
        getWorld()->pitDepleted();
        setAliveStatus(false);
        return;
    }
    int chance = randInt(0, 49);
    if(chance != 0){return;}
    Actor* bacterium;
    chance = randInt(0, 2);
    while (bacteriaCount > 0) { //may need to change this later
        if (chance == 0 && m_numSalmonella > 0) {
            bacterium = new RegularSalmonella(getX(), getY(), getWorld());
            getWorld()->addActor(bacterium);
            getWorld()->playSound(SOUND_BACTERIUM_BORN);
            m_numSalmonella--;
            break;
        } else if(chance == 1 && m_numAgroSalm > 0){
            bacterium = new AgressiveSalmonella(getX(), getY(), getWorld());
            getWorld()->addActor(bacterium);
            getWorld()->playSound(SOUND_BACTERIUM_BORN);
            m_numAgroSalm--;
            break;
        } else if(chance == 2 && numEcoli > 0){
            bacterium = new Ecoli(getX(), getY(), getWorld());
            getWorld()->addActor(bacterium);
            getWorld()->playSound(SOUND_BACTERIUM_BORN);
            numEcoli--;
            break;
        }
        bacteriaCount--;
    }
}




Projectile::Projectile(int imageID, double startX, double startY, Direction dir, int travelDist, bool alive, StudentWorld* world, bool damageable, int depth):Actor(imageID, startX, startY, alive, world, damageable, dir, false, 0, depth), m_travelDist(travelDist){
    
}

bool Projectile::attemptDamage(int amount){
    Actor* target = nullptr;
    if(getWorld()->determineOverlap(getX(), getY(), target)){
        if(target->isDamageable() && !target->isPlayer()){
            target->takeDamage(amount);
            setAliveStatus(false);
            return true;
        }
    }
    return false;
}







Spray::Spray(int imageID, double startX, double startY, Direction dir, int travelDist, bool alive, StudentWorld* world, bool damageable, int depth):Projectile(imageID, startX, startY, dir, travelDist, alive, world, damageable, depth){
}

void Spray::doSomething(){
    if(!isAlive()){return;}
    if(attemptDamage(SPRAY_DAMAGE_AMOUNT)) return; //we hit something
    //check for collision, decrement object's health,
    //set alive to false, return
    move();
    
}






Flame::Flame(int imageID, double startX, double startY, Direction dir, int travelDist, bool alive, StudentWorld* world, bool damageable, int depth):Projectile(imageID, startX, startY, dir, travelDist, alive, world, damageable, depth)
{
    
}

void Flame::doSomething(){
    if(!isAlive()) return;
    if(attemptDamage(FLAME_DAMAGE_AMOUNT)) return;
    move();
}






Item::Item(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth):Actor(imageID, startX, startY, alive, world, false, dir, false, 0, depth), m_lifetime(lifetime){
    
}

Actor* Item::checkCollisions(int increaseScore, bool fungus){
    Actor* actor = nullptr;
    if (getWorld()->determineOverlap(getX(), getY(), actor) && actor->isPlayer()) {
        getWorld()->increaseScore(increaseScore);
        setAliveStatus(false);
        if(fungus)
            getWorld()->playSound(SOUND_PLAYER_HURT);
        else
            getWorld()->playSound(SOUND_GOT_GOODIE);
        return actor;
    } else
        return nullptr;
}

RestoreHealth::RestoreHealth(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth):Item(imageID, startX, startY, dir, alive, world, lifetime, depth){
    
}

void RestoreHealth::doSomething(){
    if (!isAlive()){return;}
    Actor* actor = checkCollisions(RESTORE_HEALTH_SCORE, false);
    if (actor != nullptr){
        actor->heal(); //preform action
        return;
    }
    decrementLifetime();
    return;
}



FlamethrowerItem::FlamethrowerItem(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth):Item(imageID, startX, startY, dir, alive, world, lifetime, depth){
    
}

void FlamethrowerItem::doSomething(){
    if (!isAlive()){return;}
    Actor* actor = checkCollisions(FLAMETHROWER_GOODIE_SCORE, false);
    if (actor != nullptr){
        getWorld()->increaseFlames(5); //preform action
        return;
    }
    decrementLifetime();
    return;
}



ExtraLife::ExtraLife(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth):Item(imageID, startX, startY, dir, alive, world, lifetime, depth){
    
}

void ExtraLife::doSomething(){
    if (!isAlive()){return;}
    Actor* actor = checkCollisions(EXTRALIFE_GOODIE_SCORE, false);
    if (actor != nullptr){
        getWorld()->incLives();
        return;
    }
    decrementLifetime();
    return;
}

Fungus::Fungus(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth):Item(imageID, startX, startY, dir, alive, world, lifetime, depth){
    
}

void Fungus::doSomething(){
    if (!isAlive()){return;}
    Actor* actor = checkCollisions(FUNGUS_SCORE, true);
    if (actor != nullptr){
        getWorld()->damagePlayer(20);
        return;
    }
    decrementLifetime();
    return;
}



Bacteria::Bacteria(int imageID, double startX, double startY, StudentWorld* world, int movementDist, int hp):Actor(imageID, startX, startY, true, world, true, 90, false, hp, 0, 1.0, false, false, true){
    
}

void Bacteria::eatFood(){
    Actor* target = nullptr;
    bool food = getWorld()->determineOverlap(getX(), getY(), target);
    //cout << "isFood?: " << boolalpha << target->isFood() << endl;
    if(food && target->isFood()){
        //cout << "eating food... yum\n";
        m_foodEaten++;
        target->setAliveStatus(false);
    }
}

void Bacteria::aimTowards(Actor* obj){
    int objX, objY;
    if(obj == nullptr){ //point to player by default
        getWorld()->getPlayerCoords(objX, objY);
    } else {
        objX = obj->getX();
        objY = obj->getY();
    }
    double PI = 4 * atan(1);
    double convert = 180/PI;
    double diffX = objX - getX();
    double diffY = objY - getY();
    double angle = atan(diffY/diffX) * convert;
    if(diffX < 0 && diffY < 0) angle += 180;
    else if (diffX < 0) angle += 180;
    setDirection(angle);
}



Salmonella::Salmonella(double startX, double startY, StudentWorld* world, int movementDist, int hp):Bacteria(IID_SALMONELLA, startX, startY, world, movementDist, hp){
    
}

bool Bacteria::inBounds(int x, int y){
    return getWorld()->calcDist(x, y, VIEW_RADIUS, VIEW_RADIUS) < VIEW_RADIUS;
}

bool Bacteria::blockedByDirt(int x, int y){
    bool blockedInDirection = false;
    Actor* target = nullptr;
   
    blockedInDirection = getWorld()->determineBlockingOverlap(x, y, target);
    return blockedInDirection;
}

bool Bacteria::canMoveForward(){
    double attemptedX, attemptedY;
    getPositionInThisDirection(getDirection(), 3, attemptedX, attemptedY);
    if(!blockedByDirt(attemptedX, attemptedY) && inBounds(attemptedX, attemptedY)){
        return true;
    }
    return false;
}

void Salmonella::attemptMoveForward(){
    Actor* target;
    if(getMovementPlan() > 0){
        decrementMovement();
        if(canMoveForward()){
            moveForward(3);
        } else{
            moveRandom();
        }
    } else{
        if(getWorld()->foodNearMe(this, target)){
            aimTowards(target);
            if (canMoveForward()) {
                moveForward(3);
            } else
                moveRandom();
        } else
            moveRandom();
    }
}


void Salmonella::moveRandom(){
    int newDir = randInt(0, 359);
    setDirection(newDir);
    setMovePlanDist(10);
}

bool Bacteria::divide(bool isRegular, bool isEcoli){
    if(foodEaten() == 3){
        int newx = getX();
        if(newx < VIEW_RADIUS)
            newx += SPRITE_RADIUS;
        else if(newx > VIEW_RADIUS)
            newx -= SPRITE_RADIUS;
        int newy = getY();
        if(newy < VIEW_RADIUS)
            newy += SPRITE_RADIUS;
        else if(newy > VIEW_RADIUS)
            newy -= SPRITE_RADIUS;
        if(isRegular){
            getWorld()->addActor(new RegularSalmonella(newx, newy, getWorld()));
        }else if(isEcoli){
            getWorld()->addActor(new Ecoli(newx, newy, getWorld()));
        } else{
            getWorld()->addActor(new AgressiveSalmonella(newx, newy, getWorld()));
        }
        resetFood();
        return true;
    
    }
    return false;
}



bool Salmonella::takeDamage(int amt){ //returns true if object dead
    Actor::takeDamage(amt);
    getWorld()->playSound(SOUND_SALMONELLA_HURT);
    if(getHp() <= 0) {
        setAliveStatus(false);
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
        getWorld()->increaseScore(100);
        int chance = randInt(0, 1);
        if(chance == 0){
            getWorld()->addActor(new Food(IID_FOOD, getX(), getY(), true, getWorld()));
        }
        return true;
    }
    return false;
}


RegularSalmonella::RegularSalmonella(int startX, int startY, StudentWorld* world):Salmonella(startX, startY, world, 0, 4){
    
}

void RegularSalmonella::doSomething(){
    if(!isAlive())return;
    Actor* target = nullptr;
    bool overlapsSomething = getWorld()->determineOverlap(getX(), getY(), target);
    if(overlapsSomething && target != nullptr && target->isPlayer()){
        target->takeDamage(1);
        //skip to step 5
        attemptMoveForward();
        return;
    }
    if(divide(true)){attemptMoveForward(); return;}
    if(overlapsSomething && target->isFood())
        eatFood();
    attemptMoveForward();
}


AgressiveSalmonella::AgressiveSalmonella(int startX, int startY, StudentWorld* world): Salmonella::Salmonella(startX, startY, world, 0, 4){
   
}

void AgressiveSalmonella::doSomething(){
    if(!isAlive()){return;}
    bool skipMove = false;
    Actor* target = nullptr;
    bool collided = getWorld()->determineOverlap(getX(), getY(), target);
    //Actor* player = getWorld()->getPlayer();
    double dist = getWorld()->calcDistFromPlayer(getX(), getY());
    if (dist <= 72) {
        skipMove = true;
        aimTowards(nullptr);
        if(canMoveForward()){
            moveForward(3);
        }
        if(collided && target->isPlayer()){
            getWorld()->damagePlayer(2);
        }
    }
    divide(false);
    if(collided && target->isFood())
        eatFood();
    if(!skipMove)
        attemptMoveForward();
}


Ecoli::Ecoli(int startX, int startY, StudentWorld* world):Bacteria(IID_ECOLI, startX, startY, world, 0, 5){
 
}

void Ecoli::move(){
    double x, y;
    if(getWorld()->calcDistFromPlayer(getX(), getY()) <= 256){
        aimTowards(nullptr); //aims toward player by default
        getPositionInThisDirection(getDirection(), 2, x, y);
        for (int i = 0; i < 10; i++) {
            if (canMoveForward()) {
                moveForward(2);
                break;
            } else if(blockedByDirt(x, y)){
                //we ran into drt
                setDirection(getDirection()+10);
            }
        }
    }
}

bool Ecoli::takeDamage(int amt){
    Actor::takeDamage(amt);
    getWorld()->playSound(SOUND_ECOLI_HURT);
    if(getHp() <= 0) {
        setAliveStatus(false);
        getWorld()->playSound(SOUND_ECOLI_DIE);
        getWorld()->increaseScore(100);
        int chance = randInt(0, 1);
        if(chance == 0){
            getWorld()->addActor(new Food(IID_FOOD, getX(), getY(), true, getWorld()));
        }
        return true;
    }
    return false;
}


void Ecoli::doSomething(){
    if(!isAlive()){return;}
    Actor* target;
    bool overlaps = getWorld()->determineOverlap(getX(), getY(), target);
    if(overlaps && target->isPlayer()){
        getWorld()->damagePlayer(4);
        move();
        return;
    }
    if(divide(false, true)){move(); return;}
    move();
    
}
