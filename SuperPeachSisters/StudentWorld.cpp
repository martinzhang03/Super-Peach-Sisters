#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"

#include <string>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    Level thislevel(assetPath());
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(2);
    oss << "level0" << getLevel()  << ".txt";
    string it = oss.str();
    Level :: LoadResult result = thislevel.loadLevel(it);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        for(int x=0; x<GRID_HEIGHT; ++x)
        {
            for(int y=0; y<GRID_WIDTH; ++y)
            {
                Level::GridEntry ge;
                ge = thislevel.getContentsOf(x, y); // x=5, y=10
                switch (ge)
                {
                    case Level::piranha:
                        list_actors.push_back(new Piranha(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        break;
                    case Level::mario:
                        list_actors.push_back(new Mario(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        break;
                    case Level::empty:
                        //cout << "Location 5,10 is empty" << endl;
                        break;
                    case Level::pipe:
                        list_actors.push_back(new Pipe(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        break;
                    case Level::koopa:
                        list_actors.push_back(new Koopa(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        //cout << "Location 5,10 starts with a koopa" << endl;
                        break;
                    case Level::goomba:
                        list_actors.push_back(new Goomba(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        //cout << "Location 5,10 starts with a goomba"<< endl;
                        break;
                    case Level::peach:
                        mypeach = new Peach(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH);
                        if(mypeach->checkalive())
                        {
                            cerr<<"Alive";
                        }
                        //cout << "Location 5,10 is where Peach starts" << endl;
                        break;
                    case Level::flag:
                        list_actors.push_back(new Flags(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        //cout << "Location 5,10 is where a flag is" << endl;
                        break;
                    case Level::block:
                        list_actors.push_back(new Block(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH, 0));
                        //cout << "Location 5,10 holds a regular block" << endl;
                        break;
                    case Level::star_goodie_block:
                        list_actors.push_back(new Block(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH, 3));
                        //cout << "Location 5,10 has a star goodie block"<< endl;
                        break;
                    case Level::flower_goodie_block:
                        list_actors.push_back(new Block(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH, 1));
                        //cout << "Location 5,10 has a star goodie block"<< endl;
                        break;
                    case Level::mushroom_goodie_block:
                        list_actors.push_back(new Block(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH, 2));
                        //cout << "Location 5,10 has a star goodie block"<< endl;
                        break;
                }
            }
        }
        
    }
        // etc...
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
//    decLives();
    ostringstream oss;
    oss << " Lives: " << getLives();
    oss << " Level: 0" << getLevel();
    int score = getScore();
    string to = "00000";
    if(score>=1000)to = "00";
    else if(score>=100)to = "000";
    else if(score>=10)to = "0000";
    oss << " Points: "<< to << getScore();
    if(mypeach->isinvin()) oss<<"StarPower!";
    if(mypeach->hasshoot()) oss<<" ShootPower!";
    if(mypeach->hasjump()) oss<<" JumpPower!";
    string s  = oss.str();
    setGameStatText(s);
    mypeach->doSomething();
    for(int i=0; i<list_actors.size(); ++i)
    {
        list_actors[i]->doSomething();
    }
    if(!mypeach->checkalive()) {playSound(SOUND_PLAYER_DIE);decLives();return GWSTATUS_PLAYER_DIED;}
    if(gameover == true){gameover = false;playSound(SOUND_GAME_OVER); return GWSTATUS_PLAYER_WON;}
    if(level_finished == true){level_finished = false;playSound(SOUND_FINISHED_LEVEL); return GWSTATUS_FINISHED_LEVEL;}
    std::vector<Actor*>::iterator it = list_actors.begin();
    it = list_actors.begin();
        while (it != list_actors.end()) {
            if ((*it)->checkalive() == false) {
                delete (*it);
                it = list_actors.erase(it);
            }
            else {
                it++;
            }
        }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete mypeach;
    std::vector<Actor*>::iterator it = list_actors.begin();
        while (it != list_actors.end()) {
            delete (*it);
            it = list_actors.erase(it);
        }
}

bool StudentWorld :: isBlocked(int x, int y)
{
    std::vector<Actor*>::iterator it = list_actors.begin();
    while (it != list_actors.end())
    {
        if((*it)->isblock())
        {
            double mx =  (*it)->getX();
            double my =  (*it)->getY();
            double displacement_X = x - mx;
            double displacement_Y = y - my;
            if(-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= displacement_Y && displacement_Y <= SPRITE_HEIGHT - 1)
            {
//                list_actors[i]->getBonked();
                return true;
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld :: isDamaged(int x, int y)
{
    for(int i=0; i< list_actors.size(); ++i)
    {
        if((list_actors[i]->isgoomba() || list_actors[i]->iskoopa() || list_actors[i]->ispiranha()) && list_actors[i]->checkalive())
        {
            double mx =  list_actors[i]->getX();
            double my =  list_actors[i]->getY();
            double displacement_X = x - mx;
            double displacement_Y = y - my;
            if(-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= displacement_Y && displacement_Y <= SPRITE_HEIGHT - 1)
            {
//                list_actors[i]->getBonked();
                return true;
            }
        }
    }
    return false;
}

void StudentWorld :: damageat(int x, int y)
{
    for(int i=0; i< list_actors.size(); ++i)
    {
        if((list_actors[i]->iskoopa() || list_actors[i]->isgoomba() || list_actors[i]->ispiranha()) && list_actors[i]->checkalive())
        {
            double mx =  list_actors[i]->getX();
            double my =  list_actors[i]->getY();
            double displacement_X = x - mx;
            double displacement_Y = y - my;
            if(-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= displacement_Y && displacement_Y <= SPRITE_HEIGHT - 1)
            {
                list_actors[i]->setdeath();
                if(list_actors[i]->iskoopa())
                {
                    introduce(new Shell(this, list_actors[i]->getX(), list_actors[i]->getY(), list_actors[i]->getDirection()));
                    increaseScore(100);
                }
                else if(list_actors[i]->ispiranha())
                {
                    increaseScore(100);
                }
                else if(list_actors[i]->isgoomba())
                {
                    increaseScore(100);
                }
            }
        }
    }
}

void StudentWorld :: bonkat(int x, int y)
{
    for(int i=0; i< list_actors.size(); ++i)
    {
        if(list_actors[i]->isblock() && list_actors[i]->checkalive())
        {
            double mx =  list_actors[i]->getX();
            double my =  list_actors[i]->getY();
            double displacement_X = x - mx;
            double displacement_Y = y - my;
            if(-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= displacement_Y && displacement_Y <= SPRITE_HEIGHT - 1)
            {
                list_actors[i]->getBonked();
            }
        }
    }
}

void StudentWorld :: introduce(Actor* actor)
{
    list_actors.push_back(actor);
}

bool StudentWorld :: overlap_peach(int x, int y)
{
    double px =  mypeach->getX();
    double py =  mypeach->getY();
    double displacement_X = x - px;
    double displacement_Y = y - py;
    if(-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= displacement_Y && displacement_Y <= SPRITE_HEIGHT - 1)
    {
        return true;
    }
    return false;
}

void StudentWorld::increase_hp()
{
    mypeach->hitpoint_up();
}
