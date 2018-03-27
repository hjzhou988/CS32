#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>
#include <algorithm>
#include <queue>
#include <stack>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0): GraphObject(imageID, startX, startY, dir, size, depth)
{
    setVisible(true);
}
Actor::Actor(StudentWorld* s,int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0): GraphObject(imageID, startX, startY, dir, size, depth)
{
    setVisible(true);
    p=s;
}
void Actor::move(Direction d)
{
    if (d== down)
    {
        setDirection(down); moveTo(getX(), getY()-1);
    }
    if(d==up)
    {
        setDirection(up);moveTo(getX(), getY()+1);
    }
    if (d==left)
    {
        setDirection(left);moveTo(getX()-1, getY());
    }
    if (d==right)
    {
        setDirection(right);moveTo(getX()+1, getY());
    }
}
void Actor::moveinthecurrentdirection()
{
    Direction d= getDirection();
    if (d== down)
    {
        moveTo(getX(), getY()-1);
    }
    if(d==up)
    {
        moveTo(getX(), getY()+1);
    }
    if (d==left)
    {
        moveTo(getX()-1, getY());
    }
    if (d==right)
    {
        moveTo(getX()+1, getY());
    }
}
void Actor::moveonestep(int x, int y) // helper function for findshortestpath
{
    if (x>getX())
        setDirection(right);
    if (x<getX())
        setDirection(left);
    if (y>getY())
        setDirection(up);
    if (y<getY())
        setDirection(down);
    moveTo(x,y);
}
bool Actor::checkblock(Direction dir, int x, int y) // check if it is blocked by ice or boulder or boundary. return true when there is a block
{
    if (checkboundaryblock(dir, x, y) ||
    checkiceblock(dir, x, y) ||
    checkboulderblock(dir, x, y)) return true;
    return false;
}
bool Actor:: checkboulderblock (Direction dir, int x, int y)
{
    if (dir==right)
    {
        if (getWorld()->checkboulderblock(x+1, y,right)) return true;
        return false;
    }
    if (dir==left)
    {
        if (getWorld()->checkboulderblock(x-1, y,left)) return true;
        return false;
    }
    if (dir==up)
    {
        if (getWorld()->checkboulderblock(x, y+1,up)) return true;
        return false;
    }
    else //(dir==down)
    {
        if (getWorld()->checkboulderblock(x, y-1,down)) return true;
        return false;
    }
}
bool Actor:: checkboundaryblock(Direction dir, int x, int y)
{
    if (dir==right)
    {
        if (x>=60) return true;
        return false;
    }
    if (dir==left)
    {
        if (x<=0) return true;
        return false;
    }
    if (dir==up)
    {
        if (y>=60) return true;
        return false;
    }
    else //(dir==down)
    {
        if (y<=0) return true;
        return false;
    }
}
bool Actor::checkiceblock(Direction dir, int x, int y) //check ice and boundary block
{
    if (dir==right)
    {
        if (x>=60) return true;
        for (int j=0;j<4;j++)
        {
            if (getWorld()->get_ice_array(x+4, y+j)!=nullptr)
                return true;
        }
        return false;
    }
    if (dir==left)
    {
        for (int j=0;j<4;j++)
        {
            if (getWorld()->get_ice_array(x-1, y+j)!=nullptr)
                return true;
        }
        
        return false;
    }
    if (dir==up)
    {
        
        for (int i=0;i<4;i++)
        {
            if (getWorld()->get_ice_array(x+i, y+4)!=nullptr)
                return true;
        }

        return false;
    }
    else //if (dir==down)
    {
        
        for (int i=0;i<4;i++)
        {
            if (getWorld()->get_ice_array(x+i, y-1)!=nullptr)
                return true;
        }
        return false;
    }
}

Squirt::Squirt(StudentWorld * pointer, int x, int y, Direction d): Actor(pointer, IID_WATER_SPURT,x,y,d,1.0,1)
{
    SetCountdown(4);
    setState(stable);
    if (checkblock(d,x,y)) setState(dead);
}
void Squirt::doSomething()
{
    if (getWorld()->squirtannoyprotesters(getX(), getY())){setState(dead); return ;}
    if (Timeup()) {setState(dead); return ;}
    if (checkblock(getDirection(),getX(),getY())) {setState(dead); return ;}
    move(getDirection());
    CountminusOne();
    return;
}

bool Baggets::bediscovered()
{
    if (!amIvisible())
    {
        if ( distancetoiceman() <= 4.0)
        { setVisible(true);
            setvisibility(true);
            return true;}
    }
    return false;
}
Barrel::Barrel(StudentWorld* p, int x, int y):Baggets(p,x,y,IID_BARREL,false,stable)
{
    setvisibility(false);
}
void Barrel::doSomething()
{
    if (getState()==dead) return;
    
    if (bediscovered())
        return;
    
    if ( distancetoiceman() <= 3.0)
    {
        setState(dead);
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->decreaseBarrelsleft();
        return;
    }
}


GoldNugget::GoldNugget(StudentWorld *p, int x, int y, State s, bool b): Baggets(p,x, y, IID_GOLD,b,s)
{
    setvisibility(b);
    if (s==pickablebyprotesters)
    {
        SetCountdown(100);
    }
}
void GoldNugget::doSomething()
{
    if (getState()==dead) return;
    
    if (getState()==pickablebyiceman)
    {
        if (bediscovered())
           return;
        if ( distancetoiceman() <= 3.0 )
        {
            setState(dead);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(10);
            getWorld()->increaseIcemanGold();
            return;
        }
    }
    if (getState()==pickablebyprotesters)
    {
        if (Timeup())
        {
            setState(dead);
        }
        if (getWorld()->nuggetpickbyprotesters(getX(), getY()))
        {
            setState(dead);
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        }
        CountminusOne();
    }
    return;
}

Goodies::Goodies(StudentWorld *p, int x, int y, int imageID ): ActivatingObject(p, x, y, imageID,true, waiting) //includes sonarkit and waterpool
{
    setVisible(true);
    int n_t=300-10*(getWorld()->getLevel());
    SetCountdown(max(100, n_t));
}
void Goodies::doSomething()
{
    if (getState()==dead) return;
    if (Timeup()) {setState(dead); return;}
    if ( distancetoiceman() <= 3.0)
    {
        setState(dead);
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
    CountminusOne();
}


SonarKit::SonarKit(StudentWorld *p, int x, int y): Goodies(p, x, y, IID_SONAR){
}
void SonarKit::doSomething()
{
    Goodies::doSomething();
    if (distancetoiceman()<=3.0)
    {
        getWorld()->increaseScore(75);
        getWorld()->increaseIcemanSonar();
    }
}

WaterPool::WaterPool(StudentWorld *p, int x, int y): Goodies(p, x, y, IID_WATER_POOL){}
void WaterPool::doSomething()
{
    Goodies::doSomething();
    if (distancetoiceman()<=3.0)
    {
        getWorld()->increaseScore(100);
        getWorld()->increaseIcemanSquirts();
    }
}

Protester::Protester(StudentWorld *p, int imageID, unsigned int health):Agent (p, 60,60, left, imageID, health )
{
    setState(waiting);//ie, resting state
    setVisible(true);
    shout=15;
    numSquaresToMoveInCurrentDirection=rand()%53+8;
    int ttwbm=3-getWorld()->getLevel()/4;
    ticksToWaitBetweenMoves=max(0,ttwbm);
    restingTickCounts=ticksToWaitBetweenMoves;
    perpendicularturn=0;  // in the last 200 non-resting ticks, has not made any perpendicular turns.
}
void Protester::shoutaticeman()
{
    getWorld()->playSound(SOUND_PROTESTER_YELL);
    getWorld()->hiticeman();
    return;
}
void Protester::resetRestingtickcounts(int v)
{
    restingTickCounts=v;
}
bool Protester::findshortestpath(int endx,int endy, stack<Node*> & s) // endx and endy are the cooridinates of the endpoint
{
    //initialize visited table
    for (int i=0;i<64;i++)
        for (int j=0;j<64;j++)
            visited[i][j]=false;
    //initialize prev table
    for (int i=0;i<61;i++)
        for (int j=0;j<64;j++){
            prev[i][j]= nullptr;}
    //initialize nodes
    for (int i=0;i<64;i++)
        for (int j=0;j<64;j++){
            nodes[i][j].x=i; nodes[i][j].y=j;}

    Node* origin = & nodes[getX()][getY()];
    queue<Node*> Q;
    Q.push(origin);
    Node* current = origin;
    while (!Q.empty())
    {
        current=Q.front();
        Q.pop();
        if (!checkblock(up, current->x, current->y) && visited[current->x][current->y+1]==false)
        {
            Q.push(&nodes[current->x][current->y+1]);//add node into Q.
            visited[current->x][current->y+1]=true;
            prev[current->x][current->y+1]=current;
            if (current->x==endx && current->y+1==endy)
            {
                current=&nodes[endx][endy];
                break;
            }
        }
        if (!checkblock(down, current->x, current->y) && visited[current->x][current->y-1]==false)
        {
            Q.push(&nodes[current->x][current->y-1]);
            visited[current->x][current->y-1]=true;
            prev[current->x][current->y-1]=current;
            if (current->x==endx && current->y-1==endy)
            {current=&nodes[endx][endy];
                break;
            }
        }
        if (!checkblock(right, current->x, current->y) && visited[current->x+1][current->y]==false)
        {
            Q.push(&nodes[current->x+1][current->y]);
            visited[current->x+1][current->y]=true;
            prev[current->x+1][current->y]=current;
            if (current->x+1==endx && current->y==endy)
            {current=&nodes[endx][endy];
                break;}
        }
        if (!checkblock(left, current->x, current->y)&& visited[current->x-1][current->y]==false)
        {
            Q.push(&nodes[current->x-1][current->y]);
            visited[current->x-1][current->y]=true;
            prev[current->x-1][current->y]=current;
            if (current->x-1==endx && current->y==endy)
            {current=&nodes[endx][endy];
                break;}
        }
    }
  
    if (current!=&nodes[endx][endy]) // if did not find shortest path
    {//getWorld()->playSound(SOUND_FALLING_ROCK);
        return false;}
    else//found shortest path
    {
        //stack<Node*> S; // to hold the node pointers from end to origin to exit
        while (current!= &nodes[getX()][getY()])
        {
            s.push(current);
            current=prev[current->x][current->y];
        }
        //getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        return true;
    }
}

bool Protester::doSomethingpart1()
{
    if (getState()==dead) return true;
    
    if (getState()==waiting) // ie, resting state
    {
        if (restingTickCounts>0)
        {
            restingTickCounts--;
            return true;
        }
        else
        {
            restingTickCounts=ticksToWaitBetweenMoves;
        }
    }
    if (getState()==falling)//ie, leaving the oil field state.
    {
        if (getX()==60 && getY()==60)
        {setState(dead);return true;}
        if(S.empty()==true)
        {findshortestpath(60, 60,S); return true;}
        if (!S.empty())
        {
            moveonestep(S.top()->x,S.top()->y);
            S.pop();
        }
        return true;
    }
    
    //check if within a distance of 4 units
    if (shout>=15)
    {
        if (getX()-getWorld()->getIcemanPosX()>=0 && getX()-getWorld()->getIcemanPosX()<=4 && getY()-getWorld()->getIcemanPosY()>=0 && getY()-getWorld()->getIcemanPosY()<=4)
            if (getDirection()==left || getDirection()==down)
            {
                shoutaticeman();
                shout=0;
                perpendicularturn++;
                return true;
            }
        if (getX()-getWorld()->getIcemanPosX()>=0 && getX()-getWorld()->getIcemanPosX()<=4 && getY()-getWorld()->getIcemanPosY()<=0 && getY()-getWorld()->getIcemanPosY()>=-4)
            if (getDirection()==left || getDirection()==up)
            {
                shoutaticeman();
                shout=0;
                perpendicularturn++;
                return true;
            }
        if (getX()-getWorld()->getIcemanPosX()<=0 && getX()-getWorld()->getIcemanPosX()>=-4 && getY()-getWorld()->getIcemanPosY()>=0 && getY()-getWorld()->getIcemanPosY()<=4)
            if (getDirection()==right || getDirection()==down)
            {
                shoutaticeman();
                shout=0;
                perpendicularturn++;
                return true;
            }
        if (getX()-getWorld()->getIcemanPosX()<=0 && getX()-getWorld()->getIcemanPosX()>=-4 && getY()-getWorld()->getIcemanPosY()<=0 && getY()-getWorld()->getIcemanPosY()>=-4)
            if (getDirection()==right || getDirection()==up)
            {
                shoutaticeman();
                shout=0;
                perpendicularturn++;
                return true;
            }
    }
    return false;
    
}
bool Protester::doSomethingpart2()
{
    //if protester is in a straight line with iceman
    bool whetherclearpath=false;//check whether the path is clear or not. default is false.
    int whichdirection=0;
    if(getX()==getWorld()->getIcemanPosX()) // they are vertical to each other.
    {
        if (getY()>(getWorld()->getIcemanPosY()+4)) // protester is above iceman
        {
            
            if (!getWorld()->checkiceinarea(getX(), getX()+4, getWorld()->getIcemanPosY()+4, getY()))
                
                if (!getWorld()->checkboulderinanarea(getX()-4, getX()+4, getWorld()->getIcemanPosY()+1, getY()-1))
                    whetherclearpath=true;
            whichdirection=1; //
            
        }
        if (getY()<(getWorld()->getIcemanPosY()-4))// protester is below iceman
        {
            
            if (!getWorld()->checkiceinarea(getX(), getX()+4, getY()+4,getWorld()->getIcemanPosY()))
                
                if (!getWorld()->checkboulderinanarea(getX()-4, getX()+4, getY()+1,getWorld()->getIcemanPosY()-1))
                    whetherclearpath=true;
            whichdirection=2;
        }
    }
    if (getY()==getWorld()->getIcemanPosY()) // they are horizontal to each other
    {
        if (getX()>(getWorld()->getIcemanPosX()+4)) //protester is on the right
        {
            if (!getWorld()->checkiceinarea(getWorld()->getIcemanPosX()+4, getX(), getY(), getY()+4))
                if (!getWorld()->checkboulderinanarea(getWorld()->getIcemanPosX()+1, getX()-1, getY()-4, getY()+4))
                    whetherclearpath=true;
            whichdirection=3;
        }
        if (getX()<(getWorld()->getIcemanPosX()-4))/// protester is on the left
        {
            if (!getWorld()->checkiceinarea(getX()+4, getWorld()->getIcemanPosX(), getY(), getY()+4))
                
                if (!getWorld()->checkboulderinanarea(getX()+1, getWorld()->getIcemanPosX()-1, getY()-4, getY()+4))
                    whetherclearpath=true;
            whichdirection=4;
        }
    }
    if (whetherclearpath==true) // when the path is clear
    {
        switch (whichdirection)
        {
            case 1: setDirection(down);break;
            case 2: setDirection(up);break;
            case 3: setDirection(left);break;
            case 4: setDirection(right);break;
        }
        moveinthecurrentdirection();
        numSquaresToMoveInCurrentDirection=0;
        shout++;
        perpendicularturn++;
        //getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        return true;
    }
    
    //Regular protester can't directly see the iceman. move in the direction with number of squares
    numSquaresToMoveInCurrentDirection--;
    
    //if finished current numSquaresToMoveInCurrentDirection.
    if (numSquaresToMoveInCurrentDirection<=0)
    {
        int randomdir = rand()%4;
        bool blocked=true;
        
        while (blocked)
        {
            switch (randomdir)
            {
                case 0:
                    if (!checkblock(up,getX(),getY())) blocked=false;
                    else randomdir = rand()%4;
                    break;
                case 1:
                    if (!checkblock(right,getX(),getY())) blocked=false;
                    else randomdir = rand()%4;
                    break;
                case 2:
                    if (!checkblock(left,getX(),getY())) blocked=false;
                    else randomdir = rand()%4;
                    break;
                case 3:
                    if (!checkblock(down,getX(),getY())) blocked=false;
                    else randomdir = rand()%4;
                    break;
                default:
                    break;
            }
        }
        
        switch (randomdir)
        {
            case 0: setDirection(up);break;
            case 1: setDirection(right);break;
            case 2: setDirection(left);break;
            case 3: setDirection(down);break;
        }
        //if (checkblock(getDirection()))
        numSquaresToMoveInCurrentDirection=rand()%53+8; //set a new numSquaresToMoveInCurrentDirection
    }
    // if the protester has not finished the current numSquaresToMoveInCurrentDirection
    else
    {
        //Check if it is at an intersection, if has not made any perpendicular turn in the last 200 non resting ticks
        if (perpendicularturn>=200)
        {
            Direction dir =getDirection();
            if (dir==right || dir ==left) // when the protester is facing left or right
            {
                //Check the up and down
                if (!checkblock(up,getX(),getY()) || !checkblock(down,getX(),getY())) // when either up or down is not blocked
                {
                    if (checkblock(up,getX(),getY())) // if up is blocked, choose down
                        setDirection(down);
                    else // if up is not blocked
                    {
                        if (checkblock(down,getX(),getY())) //if down is blocked
                            setDirection(up);
                        else //neither up or down is blocked
                        {
                            int b=rand()%2;
                            if (b==0)
                                setDirection(up);
                            else setDirection(down);
                        }
                    }
                    numSquaresToMoveInCurrentDirection=rand()%53+8;
                    perpendicularturn=0;
                }
            }
            else // when the protester is facing up or down
            {
                //Check the left and right
                if (!checkblock(left,getX(),getY()) || !checkblock(right,getX(),getY()))
                {
                    if (checkblock(left,getX(),getY())) // if left is blocked, choose right
                        setDirection(right);
                    else // if left is not blocked
                    {
                        if (checkblock(down,getX(),getY())) //if right is blocked
                            setDirection(left);
                        else //neither left or right is blocked
                        {
                            int b=rand()%2;
                            if (b==0)
                                setDirection(left);
                            else setDirection(right);
                        }
                    }
                    numSquaresToMoveInCurrentDirection=rand()%53+8;
                    perpendicularturn=0;
                }
            }
        }
        
        
        if (checkblock(getDirection(),getX(),getY())) //if blocked
        {
            //getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
            numSquaresToMoveInCurrentDirection=0;
            shout++;
            perpendicularturn++;
            return true;
        }
        //If there is no intersection
        moveinthecurrentdirection();//take one step in its currently facing direction
        shout++;
        perpendicularturn++;
        return true;
    }
    
    return false;

}

//HardcoreProtester::HardcoreProtester(StudentWorld *p, int x, int y):
bool Protester::decreaseHealth(int v)
{
    Agent::decreaseHealth(v);
    if (gethealth()<=0)
    {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        restingTickCounts=0;
        setState(falling);
        if (v==100) //hit by boulder
            getWorld()->increaseScore(500);
        return true;
    }//leave the field state
    else
    {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        setState(waiting);//resting state
        int n_n=100-getWorld()->getLevel()*10;
        restingTickCounts=std::max(50, n_n);
        return true;
    }
}

RegProtester::RegProtester(StudentWorld *p): Protester(p, IID_PROTESTER, 5)
{
}
bool RegProtester::decreaseHealth(int v)
{
    Protester::decreaseHealth(v);
    
    if (gethealth()<=0 && v==2)//hit by squirt
    {
        getWorld()->increaseScore(100);
    }
    return true;
}
void RegProtester::doSomething()
{
    if (Protester::doSomethingpart1())
        return;
    else
    {
        Protester::doSomethingpart2();
    }
}
void HardcoreProtester::doSomething()
{
    if (Protester::doSomethingpart1())
        return;
    else
    {
        
        if (findshortestpath(getWorld()->getIcemanPosX(),getWorld()->getIcemanPosY(),S2) && S2.size()<=M)
        {
            moveonestep(S2.top()->x, S2.top()->y);
            shoutplusplus();
            perpendicularturnplusplus();
            //getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
            while (!S2.empty()) //clean S2.
            {
                S2.pop();
            }
            return;
        }
        Protester::doSomethingpart2();
    }
}

bool HardcoreProtester::decreaseHealth(int v)
{
    Protester::decreaseHealth(v);
    if (gethealth()<=0 && v==2)//hit by squirt
    {
        getWorld()->increaseScore(250);
    }
    return true;
}
HardcoreProtester::HardcoreProtester(StudentWorld *p): Protester(p, IID_HARD_CORE_PROTESTER, 20)
{
    M=16+getWorld()->getLevel()*2;
}

Iceman::Iceman(StudentWorld *pointer): Agent(pointer, 30, 60, right,IID_PLAYER, 10 ) {
    m_sonar=1;
    m_water=5;
    m_nuggets=0;
    //sethealth(10);
}

bool Iceman::checkblock(Direction dir)
{
    if (checkboundaryblock(dir, getX(), getY())) return true;
    if (checkboulderblock(dir,getX(), getY())) return true;
    return false;
}
bool Iceman:: decreaseHealth (int v)
{
    Agent::decreaseHealth(v);
    if (gethealth()<=0)
    {
        setState(dead);
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
    }
    return true;
}
void Iceman::cleanice()
{
    for (int i=0;i<4;i++)
        for (int j=0;j<4; j++)
        {
            int I=getX()+i; int J=getY()+j;
            if (getWorld()->get_ice_array(I,J)!=nullptr)
                getWorld()->RemoveIce(I, J);
        }
}

void Iceman::doSomething()
{
    if (gethealth()<=0)
    {
        setState(dead);
        return;
    }
    int ch;
    if (getWorld()->getKey(ch)==true)
    {
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                if (getDirection()!=left) {setDirection(left); break;}
                if (checkblock(left)) {moveTo(getX(),getY());break;}
                moveTo(getX()-1,getY());
                cleanice();
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection()!=right) {setDirection(right); break;}
                if (checkblock(right)) {moveTo(getX(),getY());break;}
                moveTo(getX()+1,getY());
                cleanice();
                break;
            case KEY_PRESS_UP:
                if (getDirection()!=up) {setDirection(up); break;}
                if(checkblock(up)){moveTo(getX(),getY());break;}
                moveTo(getX(),getY()+1);
                cleanice();
                break;
            case KEY_PRESS_DOWN:
                if (getDirection()!=down) {setDirection(down); break;}
                if(checkblock(down)){moveTo(getX(),getY());break;}
                moveTo(getX(),getY()-1);
                cleanice();
                break;
            case KEY_PRESS_ESCAPE:
                sethealthto0();
                break;
            case KEY_PRESS_SPACE:
                if (m_water>0)
                {
                    m_water--;
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    if (getDirection()==down)
                    {
                        getWorld()->CreateSquirt(getX(),getY()-4, getDirection());
                        break;
                    }
                    if (getDirection()==up)
                    {
                        getWorld()->CreateSquirt(getX(),getY()+4, getDirection());
                        break;
                    }
                    if (getDirection()==right)
                    {
                        getWorld()->CreateSquirt(getX()+4,getY(), getDirection());
                        break;
                    }
                    if (getDirection()==left)
                    {
                        getWorld()->CreateSquirt(getX()-4,getY(), getDirection());
                        break;
                    }
                }
                break;
            case 'z':
            case 'Z':
            {
                if (m_sonar==0) break;
                m_sonar--;
                getWorld()->revealhidden(getX(), getY());
                getWorld()->playSound(SOUND_SONAR);
                break;
            }
                
            case KEY_PRESS_TAB:
            {
                if (m_nuggets==0) break;
                m_nuggets--;
                getWorld()->dropnuggets(getX(), getY());
                break;
            }
                
        }
    }
    return;
}

Boulders::Boulders(StudentWorld *pointer,int startx, int starty): Actor(pointer,IID_BOULDER, startx, starty, down, 1.0, 1)
{
    setState(stable);
    SetCountdown(30);
    hiticelag=1;
}

void Boulders::doSomething()
{
    //check state
    
    //moveTo(getX(), getY()-1); return;
    
    if (getState()==waiting)
    {
        if (!Timeup()) {CountminusOne(); return;}
        else {
            setState(falling);
            getWorld()->playSound(SOUND_FALLING_ROCK);
            return;
        }
    }
    if (getState()==falling)
    {
        if (checkboulderblock(down, getX(), getY()) || checkboundaryblock(down, getX(), getY()))
        {
            setState(dead);return;
        }
        if (checkiceblock(down, getX(),getY()))
        {
            if (hiticelag!=0) // there is a lag of disappearance after hitting ice.
            {
                moveinthecurrentdirection();
                hiticelag--;
                return;
            }
            else
             setState(dead);return;
        }
        else
        {
            getWorld()->bonkprotesters(getX(), getY());
            moveTo(getX(), getY()-1);
            return;
        }
    }
    if (checkiceblock(down,getX(),getY())) //as long as there is ice below, it is stable
    { //getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        return;}
    else {setState(waiting); return;}
}




