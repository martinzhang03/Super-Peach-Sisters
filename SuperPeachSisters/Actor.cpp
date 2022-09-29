#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

Actor :: Actor(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size): GraphObject(imageID, startX, startY, dir, depth, size) {thisworld = world;alive=true;}

Actor :: ~Actor(){}

bool Actor :: checkalive() const
{
    return alive;
}

void Actor :: setdeath()
{
    alive = false;
}

StudentWorld* Actor :: getWorld()
{
    return thisworld;
}

Peach :: Peach(StudentWorld* world, int startX, int startY) : Actor(world, IID_PEACH, startX, startY, 0, 0, 1)
{
    invincible = 0;
    jump_power = 0;
    shoot_power = 0;
    hitpoint=1;
    jumpdistance = 0;
    time_to_recharge_before_next_fire=0;
}

Peach :: ~Peach(){}

void Peach :: doSomething()
{
    if(checkalive() == false){cerr<<"died";return;}
    if(invincible>0) invincible--;
    if(time_to_recharge_before_next_fire>0)time_to_recharge_before_next_fire--;
    int keymove;
    
    double x = getX();
    double y = getY();
    getWorld()->isBlocked(x, y);
    if(jumpdistance>0)
    {
        int estimate_y = y + 4;
        if(getWorld()->isBlocked(x, estimate_y))
        {
            getWorld()->bonkat(x, estimate_y);
            jumpdistance = 0;
        }
        else
        {
            moveTo(x, estimate_y);
            jumpdistance--;
        }
        return;
    }
    else
    {
        int estimate_y = y-4;
        if(!getWorld()->isBlocked(x, estimate_y))
        {
            moveTo(x, estimate_y);
        }
    }
    getWorld()->getKey(keymove);
    switch(keymove)
    {
        case KEY_PRESS_LEFT:
            setDirection(180);
            x-=4;
            //if(x<0)x=0;
            if(!getWorld()->isBlocked(x,y))
                moveTo(x, y);
            else
            {
                getWorld()->bonkat(x, y);
            }
            break;
        case KEY_PRESS_RIGHT:
            setDirection(0);
            x+=4;
            //if(x>=VIEW_WIDTH)x=VIEW_WIDTH-1;
            if(!getWorld()->isBlocked(x,y))
                moveTo(x, y);
            else
            {
                getWorld()->bonkat(x, y);
            }
            break;
        case KEY_PRESS_UP:
            if(getWorld()->isBlocked(x, y-1))
            {
                if(!jump_power)jumpdistance = 8;
                else jumpdistance = 12;
                getWorld()->playSound(SOUND_PLAYER_JUMP);
            }
            break;
        case KEY_PRESS_SPACE:
            if(shoot_power!=0 && time_to_recharge_before_next_fire == 0)
            {
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                time_to_recharge_before_next_fire = 8;
                if(getDirection() == 0)
                {
                    getWorld()->introduce(new Peach_fireball(getWorld(), x+4, y, 0));//a fireball)
                }
                else
                {
                    getWorld()->introduce(new Peach_fireball(getWorld(), x-4, y, 180));
                }
            }
            break;
    }
}

void Peach :: getBonked()
{
    if(invincible!=0)return;
    invincible = 10;
    hitpoint--;
    if(shoot_power!=0)shoot_power = 0;
    if(jump_power!=0)jump_power = 0;
    if(hitpoint>0)getWorld()->playSound(SOUND_PLAYER_HURT);
    else setdeath();
}



Block :: Block(StudentWorld* world, int startX, int startY, int is_goodie) : Actor(world, IID_BLOCK, startX, startY, 0, 2, 1){type_of_goodie = is_goodie;}


void Block :: getBonked()
{
    if(type_of_goodie == 0)
        getWorld()->playSound(SOUND_PLAYER_BONK);
    else
    {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        if(type_of_goodie == 1)//1 is shoot power
        {
            getWorld()->introduce(new Flower(getWorld(), getX(), getY()+SPRITE_HEIGHT));
        }
        else if(type_of_goodie == 2)//2 is jump power
        {
            getWorld()->introduce(new Mushroom(getWorld(), getX(), getY()+SPRITE_HEIGHT));
        }
        else if(type_of_goodie == 3)//3 is star power
        {
            getWorld()->introduce(new Star(getWorld(), getX(), getY()+SPRITE_HEIGHT));
        }
        type_of_goodie = 0;
    }
}


void Block :: doSomething(){}
Block :: ~Block(){}



Pipe :: Pipe(StudentWorld* thisworld, int startX, int startY) : Actor(thisworld, IID_PIPE, startX, startY, 0, 2, 1){}
void Pipe :: getBonked()
{
    getWorld()->playSound(SOUND_PLAYER_BONK);
}

void Pipe :: doSomething(){}
Pipe :: ~Pipe(){}



Flags :: Flags(StudentWorld* thisworld, int startX, int startY) : Actor(thisworld, IID_FLAG, startX, startY, 0, 1, 1){}

void Flags :: doSomething()
{
    if(!checkalive())return;
    if(getWorld()->overlap_peach(getX(), getY()))
    {
        setdeath();
        getWorld()->increaseScore(1000);
        getWorld()->level_complete();
    }
}

Flags :: ~Flags(){}

Mario :: Mario(StudentWorld* thisworld, int startX, int startY) : Actor(thisworld, IID_MARIO, startX, startY, 0, 1, 1){}

void Mario :: doSomething()
{
    if(!checkalive())return;
    if(getWorld()->overlap_peach(getX(), getY()))
    {
        setdeath();
        getWorld()->increaseScore(1000);
        getWorld()->finish_game();
    }
}

Mario :: ~Mario(){}


Flower :: Flower(StudentWorld* thisworld, int startX, int startY) : Actor(thisworld, IID_FLOWER, startX, startY, 0, 1, 1){}

void Flower :: doSomething()
{
    int x = getX();
    int y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        getWorld()->increaseScore(50);
        getWorld()->increase_hp();
        setdeath();
        getWorld()->get_shoot_power();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    if(!getWorld()->isBlocked(x, y-2))
    {
        y-=2;
    }
    moveTo(x, y);
    int getdir = getDirection();
    if(getdir == 0)
    {
        int estimate_x = x+2;
        if(getWorld()->isBlocked(estimate_x, y))
        {
            setDirection(180);
            return;
        }
        else
        {
            moveTo(estimate_x, y);
            return;
        }
    }
    else if(getdir == 180)
    {
        int estimate_x = x-2;
        if(getWorld()->isBlocked(estimate_x, y))
        {
            setDirection(0);
            return;
        }
        else
        {
            moveTo(estimate_x, y);
            return;
        }
    }
}

Flower :: ~Flower(){}


Mushroom :: Mushroom(StudentWorld* thisworld, int startX, int startY) : Actor(thisworld, IID_MUSHROOM, startX, startY, 0, 1, 1){}

void Mushroom :: doSomething()
{
    int x = getX();
    int y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        getWorld()->increaseScore(75);
        getWorld()->increase_hp();
        setdeath();
        getWorld()->get_jump_power();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    if(!getWorld()->isBlocked(x, y-2))
    {
        y-=2;
    }
    moveTo(x, y);
    int getdir = getDirection();
    if(getdir == 0)
    {
        int estimate_x = x+2;
        if(getWorld()->isBlocked(estimate_x, y))
        {
            setDirection(180);
            return;
        }
        else
            moveTo(estimate_x, y);
    }
    if(getdir == 180)
    {
        int estimate_x = x-2;
        if(getWorld()->isBlocked(estimate_x, y))
        {
            setDirection(0);
            return;
        }
        else
            moveTo(estimate_x, y);
    }
}

Mushroom :: ~Mushroom(){}


Star :: Star(StudentWorld* thisworld, int startX, int startY) : Actor(thisworld, IID_STAR, startX, startY, 0, 1, 1){}

void Star :: doSomething()
{
    int x = getX();
    int y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        getWorld()->increaseScore(100);
        getWorld()->increase_hp();
        setdeath();
        getWorld()->get_star_power();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    if(!getWorld()->isBlocked(x, y-2))
    {
        y-=2;
    }
    moveTo(x, y);
    int getdir = getDirection();
    if(getdir == 0)
    {
        int estimate_x = x+2;
        if(getWorld()->isBlocked(estimate_x, y))
        {
            setDirection(180);
            return;
        }
        else
            moveTo(estimate_x, y);
    }
    if(getdir == 180)
    {
        int estimate_x = x-2;
        if(getWorld()->isBlocked(estimate_x, y))
        {
            setDirection(0);
            return;
        }
        else
            moveTo(estimate_x, y);
    }
}

Star :: ~Star(){}

Piranha_fireball :: Piranha_fireball(StudentWorld* thisworld, int startX, int startY, int direction) : Actor(thisworld, IID_PIRANHA_FIRE, startX, startY, direction, 1, 1){mydirection = direction;}

void Piranha_fireball :: doSomething()
{
    int x = getX();
    int y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        setdeath();
        getWorld()->damage_peach();
        return;
    }
    if(!getWorld()->isBlocked(x, y-2))
    {
        y-=2;
    }
    moveTo(x, y);
    if(mydirection == 0)
    {
        if(getWorld()->isBlocked(x+2, y))
        {
            setdeath();
            return;
        }
        else
        {
            moveTo(x+2, y);
            return;
        }
        
    }
    else if(mydirection == 180)
    {
        if(getWorld()->isBlocked(x-2, y))
        {
            setdeath();
            return;
        }
        else
        {
            moveTo(x-2, y);
            return;
        }
    }
}


Peach_fireball :: Peach_fireball(StudentWorld* thisworld, int startX, int startY, int direction) : Actor(thisworld, IID_PEACH_FIRE, startX, startY, direction, 1, 1){mydirection = direction;}

void Peach_fireball :: doSomething()
{
    int x = getX();
    int y = getY();
    if(getWorld()->isDamaged(x, y))
    {
        setdeath();
        getWorld()->damageat(x, y);
        return;
    }
    if(!getWorld()->isBlocked(x, y-2))
    {
        y-=2;
    }
    moveTo(x, y);
    if(mydirection == 0)
    {
        if(getWorld()->isBlocked(x+2, y))
        {
            setdeath();
            return;
        }
        else
        {
            moveTo(x+2, y);
            return;
        }
        
    }
    else if(mydirection == 180)
    {
        if(getWorld()->isBlocked(x-2, y))
        {
            setdeath();
            return;
        }
        else
        {
            moveTo(x-2, y);
            return;
        }
    }
}

Shell :: Shell(StudentWorld* thisworld, int startX, int startY, int direction) : Actor(thisworld, IID_SHELL, startX, startY, direction, 1, 1){mydirection = direction;}

void Shell :: doSomething()
{
    int x = getX();
    int y = getY();
    if(getWorld()->isDamaged(x, y))
    {
        getWorld()->damageat(x, y);
        setdeath();
        return;
    }
    if(!getWorld()->isBlocked(x, y-2))
    {
        y-=2;
    }
    moveTo(x, y);
    if(mydirection == 0)
    {
        if(getWorld()->isBlocked(x+2, y))
        {
            setdeath();
            return;
        }
        else
        {
            moveTo(x+2, y);
            return;
        }
        
    }
    else if(mydirection == 180)
    {
        if(getWorld()->isBlocked(x-2, y))
        {
            setdeath();
            return;
        }
        else
        {
            moveTo(x-2, y);
            return;
        }
    }
}


Goomba :: Goomba(StudentWorld* thisworld, int startX, int startY) : Actor(thisworld, IID_GOOMBA, startX, startY, 0, 0, 1){}

void Goomba :: doSomething()
{
    if(checkalive() == false)return;
    int x = getX();
    int y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        getBonked();
        return;
    }
    if(getDirection() == 0)
    {
        if(getWorld()->isBlocked(x+1, y) || !getWorld()->isBlocked(x+SPRITE_WIDTH, y-1))
            setDirection(180);
    }
    else
    {
        if(getWorld()->isBlocked(x-1, y) || !getWorld()->isBlocked(x-SPRITE_WIDTH, y-1))
            setDirection(0);
    }
    if(getDirection() == 0)
    {
        if(getWorld()->isBlocked(x+1, y))
        {
            return;
        }
        else
        {
            moveTo(x+1, y);
            return;
        }
        
    }
    else if(getDirection() == 180)
    {
        if(getWorld()->isBlocked(x-1, y))
        {
            return;
        }
        else
        {
            moveTo(x-1, y);
            return;
        }
    }
    
    
}

void Goomba :: getBonked()
{
    if(getWorld()->getPeach()->isinvin())
    {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        setdeath();
    }
    else
    {
        getWorld()->getPeach()->getBonked();
    }
}


Koopa :: Koopa(StudentWorld* thisworld, int startX, int startY) : Actor(thisworld, IID_KOOPA, startX, startY, 0, 0, 1){}

void Koopa :: doSomething()
{
    if(checkalive() == false)return;
    int x = getX();
    int y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        getBonked();
        return;
    }
    if(getDirection() == 0)
    {
        if(getWorld()->isBlocked(x+1, y) || !getWorld()->isBlocked(x+SPRITE_WIDTH, y-1))
            setDirection(180);
    }
    else
    {
        if(getWorld()->isBlocked(x-1, y) || !getWorld()->isBlocked(x-SPRITE_WIDTH, y-1))
            setDirection(0);
    }
    if(getDirection() == 0)
    {
        if(getWorld()->isBlocked(x+1, y))
        {
            return;
        }
        else
        {
            moveTo(x+1, y);
            return;
        }
        
    }
    else if(getDirection() == 180)
    {
        if(getWorld()->isBlocked(x-1, y))
        {
            return;
        }
        else
        {
            moveTo(x-1, y);
            return;
        }
    }
}

void Koopa :: getBonked()
{
    if(getWorld()->getPeach()->isinvin())
    {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        setdeath();
        getWorld()->introduce(new Shell(getWorld(), getX(), getY(), getDirection()));
    }
    else
    {
        getWorld()->getPeach()->getBonked();
    }
}

Piranha :: Piranha(StudentWorld* thisworld, int startX, int startY) : Actor(thisworld, IID_PIRANHA, startX, startY, 0, 0, 1){}

void Piranha :: doSomething()
{
    if(checkalive() == false)return;
    increaseAnimationNumber();
    int x = getX();
    int y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        getBonked();
        return;
    }
    int diff = y-getWorld()->getPeach()->getY();
    if(diff<0)diff *=-1;
    if(diff>1.5*SPRITE_HEIGHT)return;
    if(x>getWorld()->getPeach()->getX())
        setDirection(180);
    else
        setDirection(0);
    if(fire_delay>0)
    {
        fire_delay--;
        return;
    }
    else
    {
        int diffx = x-getWorld()->getPeach()->getX();
        if(diffx<0)diffx*=-1;
        if(diffx<8*SPRITE_WIDTH)
        {
            getWorld()->introduce(new Piranha_fireball(getWorld(), x, y, getDirection()));
            getWorld()->playSound(SOUND_PIRANHA_FIRE);
            fire_delay = 40;
        }
    }
}

void Piranha :: getBonked()
{
    if(getWorld()->getPeach()->isinvin())
    {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        setdeath();
        
    }
    else
    {
        getWorld()->getPeach()->getBonked();
    }
}
