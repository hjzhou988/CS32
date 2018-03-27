#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#include "StudentWorld.h"
#include <stack>
#include <algorithm>
#include <math.h>

enum State {stable=1, waiting=2, falling=3, dead=0, active=4, pickablebyiceman=5, pickablebyprotesters=6, bribed=7};
struct Node
{
    Node(int X, int Y):x(X),y(Y){}
    Node(){}
    int x;
    int y;
};
class Actor: public GraphObject
{

public:
   
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    Actor(StudentWorld* s,int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual void doSomething(){}
    virtual StudentWorld* getWorld()
    {return p;}
    //int gethealth() const {return m_hitpoints;}
    //void sethealth(int h){ m_hitpoints=h;}
    State getState(){return s;}
    void setState(State state){s=state;}
    virtual bool decreaseHealth(int v){return false;}
    void move(Direction);
    void moveinthecurrentdirection();
    bool checkblock(Direction,int,int ); // by ice or by boulder or by boundary
    bool checkboundaryblock(Direction dir, int x, int y);
    bool checkiceblock(Direction, int, int);
    bool checkboulderblock(Direction, int, int);
    void moveonestep(int,int);
    virtual bool canActorsPassThroughMe() const{return true;}// Can other actors pass through this actor?
    virtual bool revealablebysonar() const {return false;} // can it be revealed by sonar?
    virtual bool annoyable() const {return false;} // can it be annoyed?
    virtual bool canPickThingsUp() const{return false;}
    virtual void addGold(){}
    void SetCountdown(int c){countdown=c;}
    void CountminusOne(){countdown--; }
    bool Timeup(){if (countdown<=0) return true; else return false;}
    private:
    State s;
    StudentWorld *p;
    int countdown;
    
};


class Ice: public Actor
{
public:
    Ice(int startX, int startY): Actor(IID_ICE, startX, startY, right, 0.25, 3){}
    
};

class Agent: public Actor
{
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir,
          int imageID, unsigned int hitPoints): Actor (world, imageID, startX, startY, startDir, 1.0, 0)
    {
        m_hitpoints=hitPoints;
    }
    int gethealth() const {return m_hitpoints;}
    void sethealthto0(){ m_hitpoints=0;}
    virtual bool decreaseHealth(int v){m_hitpoints-=v; return true;}
    virtual bool annoyable() const {return true;} // can it be annoyed?
    virtual bool canPickThingsUp() const{return true;}
private:
    int m_hitpoints;
};

class Iceman: public Agent
{
public:
    Iceman(StudentWorld *pointer);
    virtual void doSomething();
    int getSquirts() const { return m_water;}
    int getGold() const {return m_nuggets;}
    int getSonar() const {return m_sonar;}
    void increaseGold(){m_nuggets++;}
    void increaseSonar(){m_sonar++;}
    void increaseSquirts(){m_water+=5;}
    virtual bool decreaseHealth(int v); 
    
private:
    int m_sonar;
    int m_water;
    int m_nuggets;
    virtual bool checkblock(Direction);//check boulder and boundary
    void cleanice();
};

class Boulders: public Actor
{
public:
    Boulders(StudentWorld *pointer,int startX, int startY);
    virtual void doSomething();
    virtual bool canActorsPassThroughMe() const {return false;}
private:
    int hiticelag;
};

class Squirt: public Actor
{
public:
    Squirt(StudentWorld *pointer, int x, int y, Direction d);
    virtual void doSomething();

};



class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID, bool visible, State s):Actor(world, imageID, startX, startY, right, 1.0, 2)
    {
        setVisible(visible);
        setState(s);
    }
    //virtual void doSomething()=0;
    double distancetoiceman()
    {
        int deltaX=getWorld()->getIcemanPosX()-getX();
        int deltaY=getWorld()->getIcemanPosY()-getY();
        double distance = sqrt (deltaX*deltaX+deltaY*deltaY);
        return distance;
    }
};

class Baggets: public ActivatingObject // Baggets means Barrels and Nuggets
{
public:
    Baggets(StudentWorld* world, int x, int y, int imageID,bool visible, State s): ActivatingObject(world, x, y,imageID, visible,s){}
    virtual void doSomething()=0; //
    bool bediscovered();
    virtual bool revealablebysonar() const {return true;}
    bool amIvisible(){return m_amIvisible;}
    void setvisibility (bool b) { m_amIvisible=b;}
private:
    bool m_amIvisible;
};

class Barrel: public Baggets
{
public:
    Barrel(StudentWorld *p, int,int);
    virtual void doSomething();
    };


class GoldNugget: public Baggets
{
public:
    GoldNugget(StudentWorld *p,int x,int y, State s, bool b);
    virtual void doSomething();

};

class Goodies: public ActivatingObject
{
public:
    Goodies(StudentWorld *p, int, int, int imageID);
    virtual void doSomething();
};

class SonarKit: public Goodies
{
public:
    SonarKit(StudentWorld *p, int x, int y);
    
    virtual void doSomething();
};

class WaterPool: public Goodies
{
public:
    WaterPool(StudentWorld *p, int x, int y);
    
    virtual void doSomething();
};

class Protester: public Agent
{
public:
    Protester(StudentWorld *p, int imageID, unsigned int health);
    virtual void doSomething()=0;
    virtual bool decreaseHealth(int v);
    void resetRestingtickcounts(int);
    bool findshortestpath(int,int, std::stack<Node*> &);
    //virtual bool annoy(unsigned int v);
    virtual void addGold()=0;
    bool doSomethingpart1();
    bool doSomethingpart2();
    void shoutplusplus(){shout++;}
    void perpendicularturnplusplus(){perpendicularturn++;}
private:
    int restingTickCounts;
    int numSquaresToMoveInCurrentDirection;
    int ticksToWaitBetweenMoves;
    int shout;// to record the non-resting tick count without shouting
    int perpendicularturn; // to  to record the non-resting tick count without perpendicularturn
    void shoutaticeman();
    bool visited[64][64]; //"visited" table
    Node* prev[64][64]; //"previous" table
    Node nodes[64][64]; // Nodes table
    std::stack<Node*> S; //Stack to hold the exit route
};

class RegProtester:public Protester
{
public:
    RegProtester(StudentWorld *p);
    
    virtual void addGold(){getWorld()->increaseScore(25);setState(falling);}//
    virtual bool decreaseHealth(int);
    virtual void doSomething();
};

class HardcoreProtester: public Protester
{
public:
    HardcoreProtester(StudentWorld *p);
    virtual void addGold()
    {   getWorld()->increaseScore(50);
        setState(waiting);
        int t=100-getWorld()->getLevel()*10;
        resetRestingtickcounts(std::max(50,t));
    }
    virtual bool decreaseHealth(int);
    virtual void doSomething();
private:
    std::stack<Node*> S2;//Stack to hold hard core protester chasing route
    int M;
};







#endif // ACTOR_H_
