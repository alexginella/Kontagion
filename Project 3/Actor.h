#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
using namespace std;
class StudentWorld;

const int MAX_SPRAYS = 20;
const int SPRAY_TRAVEL = 112;
const int SPRAY_DAMAGE_AMOUNT = 2;

const int AMMOUNT_FLAMES = 16;
const int FLAME_TRAVEL = 32;
const int FLAME_DAMAGE_AMOUNT = 5;

const int RESTORE_HEALTH_SCORE = 250;
const int FLAMETHROWER_GOODIE_SCORE = 300;
const int EXTRALIFE_GOODIE_SCORE = 500;
const int FUNGUS_SCORE = -50;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject{
public:
    Actor(int imageID, double startX, double startY, bool alive, StudentWorld* world, bool damageable, Direction dir = 0, bool isPlayer = false, int health = 0, int depth = 0, double size = 1.0, bool blocksStuff = false, bool isFood = false, bool isEnemy = false);
    virtual void doSomething() = 0;
    inline bool isAlive(){return m_isAlive;}
    inline void setAliveStatus(bool status){m_isAlive = status;}
    inline int getHp(){return m_hp;}
    inline StudentWorld* getWorld(){return m_world;}
    inline bool isPlayer(){return m_isPlayer;}
    inline bool isDamageable(){return m_damageable;}
    inline void heal(){m_hp = 100;}
    virtual bool takeDamage(int amount);
    inline bool blocksStuff(){return m_blocksStuff;}
    inline bool isFood(){return m_isFood;}
    inline bool isEnemy(){return m_isEnemy;}
private:
    bool m_isEnemy;
    bool m_isFood;
    bool m_blocksStuff;
    bool m_isPlayer;
    int m_hp;
    bool m_isAlive;
    bool m_damageable;
    StudentWorld* m_world;
};


class Socrates : public Actor{
public:
    Socrates(StudentWorld* world);
    inline int getSprays(){return m_sprayCharges;}
    inline int getFlames(){return m_flameCharges;}
    inline void increaseFlames(int amount){m_flameCharges+=amount;}

    void doSomething();
    
private:
    const int socratesIID = IID_PLAYER;
    Direction m_angle; //might not need this
    //int m_hp;
    int m_flameCharges;
    int m_sprayCharges;
};










class Dirt : public Actor{
public:
    Dirt(int imageID, double startX, double startY, bool alive, StudentWorld* world, bool damageable, Direction dir);
    void doSomething();
};





class Food : public Actor{
public:
    Food(int imageID, double startX, double startY, bool alive, StudentWorld* world);
    void doSomething();
    
};




class Pit : public Actor{
public:
    Pit(int imageID, double startX, double startY, StudentWorld* world, int numSalmonella, int numAgroSalm, int numEcoli);
    void doSomething();
private:
    int m_numSalmonella, m_numAgroSalm, numEcoli;
};




class Projectile : public Actor{
public:
    Projectile(int imageID, double startX, double startY, Direction dir, int travelDist, bool alive, StudentWorld* world, bool damageable, int depth);
    inline int travelDist() {return m_travelDist;}
    inline void decrementDist(int dist){m_travelDist -= dist;}
    inline void move(){
        if(travelDist() > 0){
            moveForward(SPRITE_WIDTH);
            decrementDist(SPRITE_WIDTH);
        } else {
            setAliveStatus(false);
        }
    }
    bool attemptDamage(int amount);
private:
    int m_travelDist;
    
};




class Spray : public Projectile{
public:
    Spray(int imageID, double startX, double startY, Direction dir, int travelDist, bool alive, StudentWorld* world, bool damageable, int depth);
    void doSomething();
private:
    int m_travelDist;
};








class Flame : public Projectile{
public:
    Flame(int imageID, double startX, double startY, Direction dir, int travelDist, bool alive, StudentWorld* world, bool damageable, int depth);
    void doSomething();
};







class Item : public Actor {
public:
    Item(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth);
    inline void decrementLifetime(){
        m_lifetime--;
        if(m_lifetime == 0)
            setAliveStatus(false);
    }
    inline int lifeTime(){return m_lifetime;}
    Actor* checkCollisions(int increaseScore, bool fungus);
private:
    int m_lifetime;
};






class RestoreHealth : public Item {
public:
    RestoreHealth(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth);
    void doSomething();
};





class FlamethrowerItem : public Item{
public:
    FlamethrowerItem(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth);
    void doSomething();
    
};


class ExtraLife : public Item{
public:
    ExtraLife(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth);
    void doSomething();
    
};


class Fungus : public Item{
public:
    Fungus(int imageID, double startX, double startY, Direction dir, bool alive, StudentWorld* world, int lifetime, int depth);
    void doSomething();
    
};






class Bacteria : public Actor{
public:
    Bacteria(int imageID, double startX, double startY, StudentWorld* world, int movementDist, int hp);
    inline int getMovementPlan(){return m_movementDist;}
    inline void decrementMovement(){m_movementDist--;}
    inline void setMovePlanDist(int num){m_movementDist = num;}
    inline int foodEaten(){return m_foodEaten;}
    void eatFood();
    inline void resetFood(){m_foodEaten = 0;}
    void damageSocrates(int amt);
    void aimTowards(Actor* obj);
    bool canMoveForward();
    bool blockedByDirt(int x, int y);
    bool inBounds(int x, int y);
    bool divide(bool isRegular, bool isEcoli = false);
    

private:
    int m_movementDist;
    int m_foodEaten;
    
};



class Salmonella : public Bacteria{
public:
    Salmonella(double startX, double startY, StudentWorld* world, int movementDist, int hp);
    void attemptMoveForward(); //tries moving forward
    void moveRandom(); //move in random direction
    virtual bool takeDamage(int amt);
private:
    
};



class RegularSalmonella : public Salmonella{
public:
    RegularSalmonella(int startX, int startY, StudentWorld* world);
    void doSomething();
};



class AgressiveSalmonella : public Salmonella{
public:
    AgressiveSalmonella(int startX, int startY, StudentWorld* world);
    void doSomething();
};


class Ecoli : public Bacteria{
public:
    Ecoli(int startX, int startY, StudentWorld* world);
    void doSomething();
    void move();
    virtual bool takeDamage(int amt);
};
#endif // ACTOR_H_
//Actor dirt = new Dirt()
