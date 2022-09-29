#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void bonkat(int x, int y);
    void damageat(int x, int y);
    bool isBlocked(int x, int y);
    bool isDamaged(int x, int y);
    void introduce(Actor* actor);
    bool overlap_peach(int x, int y);
    void level_complete(){level_finished = true;}
    void finish_game(){gameover = true;}
    void increase_hp();
    void get_shoot_power(){mypeach->get_shoot();}
    void get_jump_power(){mypeach->get_jump();}
    void get_star_power(){mypeach->get_star();}
    void damage_peach(){mypeach->hurt();}
    Peach* getPeach(){return mypeach;}

private:
    Peach* mypeach;
    Flags* myflag;
    vector<Actor*> list_actors;
    bool level_finished=false;
    bool gameover=false;
};

#endif // STUDENTWORLD_H_
