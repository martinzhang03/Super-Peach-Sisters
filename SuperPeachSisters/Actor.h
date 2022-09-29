#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size);
    StudentWorld* getWorld();
    virtual bool isblock(){return false;}
    virtual bool isgoomba(){return false;}
    bool checkalive() const;
    virtual bool iskoopa(){return false;}
    virtual bool ispiranha(){return false;}
    void setdeath();
    virtual void getBonked(){};
    virtual void doSomething()=0;
    virtual ~Actor();
private:
    bool alive;
    StudentWorld* thisworld;
};


class Peach : public Actor
{
public:
    Peach(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked();
    bool isinvin(){return invincible!=0;}
    bool hasjump(){return jump_power!=0;}
    bool hasshoot(){return shoot_power!=0;}
    void hitpoint_up(){hitpoint=2;}
    void get_shoot(){shoot_power = 1;}
    void get_jump(){jump_power = 1;}
    void get_star(){invincible = 150;}
    void hurt(){getBonked();}
    virtual ~Peach();
private:
    int invincible;
    int time_to_recharge_before_next_fire;
    int hitpoint;
    int jump_power;
    int shoot_power;
    int jumpdistance;
    
};

class Block : public Actor
{
public:
    Block(StudentWorld* thisworld, int startX, int startY, int is_goodie);
    void doSomething();
    bool isblock(){return true;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked();
    virtual ~Block();
private:
    int type_of_goodie;
};

class Pipe : public Actor
{
public:
    Pipe(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return true;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked();
    virtual ~Pipe();
};

class Flags : public Actor
{
public:
    Flags(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked(){}
    virtual ~Flags();
};

class Mario : public Actor
{
public:
    Mario(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked(){}
    virtual ~Mario();
};

class Flower : public Actor
{
public:
    Flower(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked(){}
    virtual ~Flower();
};

class Mushroom : public Actor
{
public:
    Mushroom(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked(){}
    virtual ~Mushroom();
};

class Star : public Actor
{
public:
    Star(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked(){}
    virtual ~Star();
};

class Piranha_fireball : public Actor
{
public:
    Piranha_fireball(StudentWorld* thisworld, int startX, int startY, int direction);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked(){}
    virtual ~Piranha_fireball(){}
private:
    int mydirection;
};

class Peach_fireball : public Actor
{
public:
    Peach_fireball(StudentWorld* thisworld, int startX, int startY, int direction);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked(){}
    virtual ~Peach_fireball(){}
private:
    int mydirection;
};

class Shell : public Actor
{
public:
    Shell(StudentWorld* thisworld, int startX, int startY, int direction);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked(){}
    virtual ~Shell(){}
private:
    int mydirection;
};

class Goomba : public Actor
{
public:
    Goomba(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return true;}
    bool iskoopa(){return false;}
    bool ispiranha(){return false;}
    void getBonked();
    virtual ~Goomba(){}
};

class Koopa : public Actor
{
public:
    Koopa(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return true;}
    bool ispiranha(){return false;}
    void getBonked();
    virtual ~Koopa(){}
};

class Piranha : public Actor
{
public:
    Piranha(StudentWorld* thisworld, int startX, int startY);
    void doSomething();
    bool isblock(){return false;}
    bool isgoomba(){return false;}
    bool iskoopa(){return false;}
    bool ispiranha(){return true;}
    void getBonked();
    virtual ~Piranha(){}
private:
    int fire_delay=0;
};
#endif // ACTOR_H_
