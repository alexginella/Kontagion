#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject{
public:
    Actor(int imageID, double startX, double startY, bool alive, StudentWorld* world, Direction dir = 0, int depth = 0, double size = 1.0);
    virtual void doSomething() = 0;
    inline bool isAlive(){return m_isAlive;}
    inline StudentWorld* getWorld(){return m_world;}
private:
    bool m_isAlive;
    StudentWorld* m_world;
};

class Socrates : public Actor{
public:
    Socrates(int imageID, double startX, double startY, bool alive, StudentWorld* world, Direction dir, Direction angle, int hp, int flameCharges, int sprayCharges);
    void doSomething();
private:
    const int socratesIID = IID_PLAYER;
    Direction m_angle; //might not need this
    int m_hp;
    int m_flameCharges;
    int m_sprayCharges;
};

class Dirt : public Actor{
public:
    Dirt(int imageID, double startX, double startY, bool alive, StudentWorld* world, Direction dir);
    void doSomething();
};

#endif // ACTOR_H_
//Actor dirt = new Dirt()
