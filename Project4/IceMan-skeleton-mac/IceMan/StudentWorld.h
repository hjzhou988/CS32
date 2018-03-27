#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <list>
#include "GraphObject.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Ice;
class Iceman;
class Boulders;
class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    virtual int init();

    virtual int move();
	
    virtual void cleanUp();

    Ice* get_ice_array(int x, int y) const {return ice_array[x][y];}
    void RemoveIce(int x, int y);
    void CreateSquirt(int x, int y, GraphObject::Direction dir);
    int getIcemanPosX();
    int getIcemanPosY();
    void decreaseBarrelsleft();
    void increaseIcemanGold();
    void increaseIcemanSonar();
    void increaseIcemanSquirts();
    void hiticeman();
    bool checkboulderblock(int, int, GraphObject::Direction);
    void revealhidden(int, int); // pass the position of iceman to it, and it reveals the hidden barrels and nuggets within 12. (center to center 12)
    void dropnuggets(int, int);
    bool nuggetpickbyprotesters(int x, int y);
    bool squirtannoyprotesters(int x, int y);
    bool checkboulderinanarea(int x1, int x2, int y1, int y2);
    bool checkiceinarea(int, int, int, int);
    void bonkprotesters(int, int);
    bool randompostionclear(int x, int y); // check whether postion x, y is clear for putting waterpool
private:
    Iceman* iceman_p;
    Actor* actor;
    std::list<Actor*> l_actors;
    Ice* ice_array[64][64];
    void setDisplayText();
    int m_barrelsleft;
    int Protesterticks;
    int num_protesters;
    int G; // chance of adding goodies
    int T; // ticks of adding protestors
    int P; // number of maximum protesters
    void addProtester();
};



#endif // STUDENTWORLD_H_
