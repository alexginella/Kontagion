#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, bool alive, StudentWorld* world, Direction dir, int depth, double size):m_world(world),  m_isAlive(alive), GraphObject(imageID, startX, startY, dir, depth){

}

Socrates::Socrates(int imageID, double startX, double startY, bool alive, StudentWorld* world, Direction dir, Direction angle, int hp, int flameCharges, int sprayCharges)
    :Actor(imageID, startX, startY, alive, world), m_angle(angle), m_hp(hp), m_flameCharges(flameCharges), m_sprayCharges(sprayCharges)
{
    
}

void Socrates::doSomething(){
    //do someting in a tick
    if(!this->isAlive()){return;}
    int ch;
    if (getWorld()->getKey(ch)) {
        switch (ch){
            case KEY_PRESS_LEFT:
                m_angle += 5;
                setDirection(getDirection()+5);
            break;
            case KEY_PRESS_RIGHT:
                m_angle -= 5;
                setDirection(getDirection()-5);
            break;
            case KEY_PRESS_SPACE:
            //... add spray in front of Socrates...;
            break;
        }
    }
}

Dirt::Dirt(int imageID, double startX, double startY, bool alive, StudentWorld* world, Direction dir):Actor(imageID, startX, startY, alive, world, dir){
    
}

void Dirt::doSomething(){
    //doesnt do anything
}
