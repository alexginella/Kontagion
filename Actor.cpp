#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <cassert>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, bool alive, StudentWorld* world, bool damageable, Direction dir, bool isPlayer, int health, int depth, double size):m_isPlayer(isPlayer), m_damageable(damageable),  m_hp(health),  m_world(world),  m_isAlive(alive), GraphObject(imageID, startX, startY, dir, depth){

}







Socrates::Socrates(int imageID, double startX, double startY, bool alive, StudentWorld* world, bool damageable, bool isPlayer, Direction dir, Direction angle, int hp, int flameCharges, int sprayCharges)
    :Actor(imageID, startX, startY, alive, world, damageable, isPlayer, dir, hp), m_angle(angle), m_flameCharges(flameCharges), m_sprayCharges(sprayCharges)
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







Dirt::Dirt(int imageID, double startX, double startY, bool alive, StudentWorld* world, bool damageable, Direction dir):Actor(imageID, startX, startY, alive, world, damageable, dir){
    
}

void Dirt::doSomething(){
    //doesnt do anything
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

Actor* Item::checkCollisions(int increaseScore){
    Actor* actor = nullptr;
    if (getWorld()->determineOverlap(getX(), getY(), actor) && actor->isPlayer()) {
        getWorld()->increaseScore(increaseScore);
        setAliveStatus(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        return actor;
    } else
        return nullptr;
}

RestoreHealth::RestoreHealth(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth):Item(imageID, startX, startY, dir, alive, world, lifetime, depth){
    
}

void RestoreHealth::doSomething(){
    if (!isAlive()){return;}
    Actor* actor = checkCollisions(RESTORE_HEALTH_SCORE);
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
    Actor* actor = checkCollisions(FLAMETHROWER_GOODIE_SCORE);
    if (actor != nullptr){
        //actor->increaseFlames(5); //preform action
        return;
    }
    decrementLifetime();
    return;
}



ExtraLife::ExtraLife(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth):Item(imageID, startX, startY, dir, alive, world, lifetime, depth){
    
}

void ExtraLife::doSomething(){
    
}

Fungus::Fungus(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth):Item(imageID, startX, startY, dir, alive, world, lifetime, depth){
    
}

void Fungus::doSomething(){
    
}

