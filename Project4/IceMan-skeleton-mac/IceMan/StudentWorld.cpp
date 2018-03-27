#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    
}
 StudentWorld::~StudentWorld()
{
    for(int i=0;i<64;i++)
        for (int j=0;j<60;j++)
            if (ice_array[i][j]!=nullptr) delete ice_array[i][j];
    delete iceman_p;
    
    list<Actor*>::iterator p=l_actors.begin();
    while (p!=l_actors.end())
    {
        delete *p;
        p=l_actors.erase(p);
    }
}

int StudentWorld::init()
{
    Protesterticks=0;
    num_protesters=0;
    G=getLevel()*25+300; // chance of adding goodies
    int n_t=200-getLevel();
    T=max(25, n_t); // ticks of adding protestors
    int n_p=2+getLevel()*1.5;
    P=min(15, n_p); //number of maximum protesters
    
    //create a Iceman objec
    iceman_p= new Iceman(this);
    
    // fill up ice
    for (int i=0;i<64;i++)
        for (int j=0;j<60;j++)
            ice_array[i][j]= new Ice(i,j);
    for (int i=0; i<64;i++)
        for (int j=60; j<64; j++)
            ice_array[i][j]= nullptr;
    // delete the middle tunnel of ice
    for (int i=30; i<34; i++)
        for (int j=4; j<60; j++)
        {delete ice_array[i][j];
            ice_array[i][j]=nullptr;
        }
    // Create boulders
    int n_b=getLevel()/2+2;
    int B= min(n_b,9);
    for (int i=0; i<B;)
    {
        int a=rand()%61;
        int b=rand()%37+20;
        if (a>26 && a<34 && b>0) continue;
        list<Actor*>::iterator p=l_actors.begin();
        for (; p!=l_actors.end();p++)
            if (sqrt(((*p)->getX()-a)*((*p)->getX()-a)+((*p)->getY()-b)*((*p)->getY()-b))<=6.0)
                break;
        if (p==l_actors.end()) // means distance greater than 6 to all objects
        {
            Boulders *boulder=new Boulders(this, a, b);
            l_actors.push_back(boulder);
            for (int i=0; i<4;i++) //delete ice behind the boulder
                for (int j=0; j<4; j++)
                {
                    if (ice_array[a+i][b+j]!=nullptr)
                    {delete ice_array[a+i][b+j]; ice_array[a+i][b+j]=nullptr; }
                }
            i++;
        }
    }
    
    //Boulders *boulder=new Boulders(this, 56,60);
    //l_actors.push_back(boulder);
    /*
    Boulders *boulder=new Boulders(this, 30,40);
    l_actors.push_back(boulder);
    
    
    boulder=new Boulders(this, 30,4);
    l_actors.push_back(boulder);*/

    
    
    //Create barrels
    int n_l=2+getLevel();
    int L=min(n_l,21);
    m_barrelsleft=L;
    for (int i=0; i<L;)
    {
        int a=rand()%61;
        int b=rand()%57;
        if (a>26 && a<34 && b>0) continue; // When barrels are created in the tunnel.
        list<Actor*>::iterator p=l_actors.begin();
        for (; p!=l_actors.end();p++)
            if (sqrt(((*p)->getX()-a)*((*p)->getX()-a)+((*p)->getY()-b)*((*p)->getY()-b))<=6.0)
                break;
        if (p==l_actors.end()) // means distance greater than 6 to all objects
        {
            Barrel *barrel =new Barrel(this, a, b);
            l_actors.push_back(barrel);
            i++;
        }
    }
    
    //Create Goldnuggests
    int n_g=5-getLevel()/2;
    int G=max(n_g,2);
    for (int i=0; i<G;)
    {
        int a=rand()%61;
        int b=rand()%57;
        if (a>26 && a<34 && b>0) continue;
        list<Actor*>::iterator p=l_actors.begin();
        for (; p!=l_actors.end();p++)
            if (sqrt(((*p)->getX()-a)*((*p)->getX()-a)+((*p)->getY()-b)*((*p)->getY()-b))<=6.0)
                break;
        if (p==l_actors.end()) // means distance greater than 6 to all objects
        {
            GoldNugget *gold=new GoldNugget(this, a, b,pickablebyiceman,false);
            l_actors.push_back(gold);
            i++;
        }
    }
    //Create a protester
    addProtester();
    
    return GWSTATUS_CONTINUE_GAME;
    
}
int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
    //incLives();
    //return GWSTATUS_PLAYER_DIED;
    
    
    setDisplayText();
    if (iceman_p->gethealth()>0)
    {
        if (m_barrelsleft==0)
        {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
        
        iceman_p->doSomething();

        list<Actor*>::iterator p=l_actors.begin();
        for (; p!=l_actors.end(); p++)
        {
            (*p)->doSomething();
        }
        
        //add new Actors
        //add new Sonar or WaterPool
        if (rand()%G==0)
        {
            if (rand()%5==0)
            {
                SonarKit *sonar=new SonarKit(this, 0,60);
                l_actors.push_back(sonar);
            }
            else
            {
                int x=rand()%61; int y=rand()%61;
                while (!randompostionclear(x, y)) //keep generating new position if not clear.
                {
                    x=rand()%61; y=rand()%61;
                }
                WaterPool *water= new WaterPool(this, x,y);
                l_actors.push_back(water);
            }
        }
        
        if (num_protesters< P && Protesterticks>=T)
        {
            addProtester();
        }
        
        //delete dead objects
        p=l_actors.begin();
         while (p!=l_actors.end())
         {
             if ((*p)->getState()==dead)
             {
                 delete (*p);
                 p=l_actors.erase(p);
                 continue;
             }
             p++;
         }
        Protesterticks++;
        return GWSTATUS_CONTINUE_GAME;
    }
    decLives();
    return GWSTATUS_PLAYER_DIED;
    
}
void StudentWorld::cleanUp()
{
    //delete the ice vector
    for(int i=0;i<64;i++)
        for (int j=0;j<60;j++)
            if (ice_array[i][j]!=nullptr) delete ice_array[i][j];
    delete iceman_p;
    
    list<Actor*>::iterator p=l_actors.begin();
    while (p!=l_actors.end())
    {
        delete *p;
        p=l_actors.erase(p);
    }
}

void StudentWorld::setDisplayText()
{
    int level=getLevel();
    int lives=getLives();
    int health= iceman_p->gethealth();
    int squirts=iceman_p->getSquirts();
    int gold=iceman_p->getGold();
    int barrelsLeft=m_barrelsleft;
    int sonar=iceman_p->getSonar();
    int score = getScore();
    
    ostringstream oss;
    oss << "Lvl: "<<setw(2) << level << "  Lives: "<<setw(1)<<lives<<"  Hlth: "<<setw(3)<<health*10<<"%  Wtr: "<<setw(2)<<squirts<<"  Gld: "<<setw(2)<<gold<<"  Oil Left: " <<setw(2)<<barrelsLeft<<"  Sonar: "<<setw(2)<<sonar<<"  Scr: ";
    oss.fill('0');
    oss <<setw(6)<< score<<endl;
    string s = oss.str();
    setGameStatText(s);
}

void StudentWorld::RemoveIce(int x, int y)
{
    delete ice_array[x][y]; ice_array[x][y]=nullptr; playSound(SOUND_DIG);
}

void StudentWorld::CreateSquirt(int x, int y, GraphObject::Direction dir)
{
    actor = new Squirt (this,x, y, dir);
    l_actors.push_back(actor);
}

int StudentWorld::getIcemanPosX()
{
    return iceman_p->getX();
}
int StudentWorld::getIcemanPosY()
{
    return iceman_p->getY();
}
void StudentWorld::decreaseBarrelsleft()
{
    m_barrelsleft--;
}
void StudentWorld::increaseIcemanGold()
{
    iceman_p->increaseGold();
}

void StudentWorld::increaseIcemanSonar()
{
    iceman_p->increaseSonar();
}
void StudentWorld::increaseIcemanSquirts()
{
    iceman_p->increaseSquirts();
}
void StudentWorld::hiticeman()
{
    iceman_p->decreaseHealth(2);
}

bool StudentWorld::checkboulderblock(int x, int y, GraphObject::Direction direction) // return true if the current direction is blocked by a boulder
{
    list<Actor*>::iterator p= l_actors.begin();
    for (;p!=l_actors.end(); p++)
    {
        if ((*p)->canActorsPassThroughMe()==false) //boulder
        {
            if (sqrt(((*p)->getX()-x)*((*p)->getX()-x)+((*p)->getY()-y)*((*p)->getY()-y))==1.0) //because I pass the coordinates shifted by 1, so the distance of 1.0 is considered the same boulder. The same boulder cannot be blocked by itself.
            {//playSound(SOUND_PROTESTER_ANNOYED);
                continue;}
            if (sqrt(((*p)->getX()-x)*((*p)->getX()-x)+((*p)->getY()-y)*((*p)->getY()-y))<=3.0)
                
            {
                
                if (direction==GraphObject::right)
                {
                    if ((*p)->getX()>=x){
                        
                        return true;
                    }
                
                }
                if (direction==GraphObject::left)
                {
                    if ((*p)->getX()<=x)
                    {
                       // playSound(SOUND_PLAYER_GIVE_UP);
                        return true;
                    }
                    
                }
                if (direction==GraphObject::up)
                {
                    if ((*p)->getY()>=y)
                    {
                       return true;
                    }
                    
                }
                if (direction==GraphObject::down)
                {
                    if ((*p)->getX()<=y)
                    {
                        return true;
                    }
                }
            }
            
        }
    }
    return false;
}
void StudentWorld::revealhidden(int x, int y) // pass the position of iceman (left bottom corner) to it, and it reveals the hidden barrels and nuggets within a radius of 12.
{
    list<Actor*>::iterator p= l_actors.begin();
    for (;p!=l_actors.end(); p++)
    {
        if ((*p)->revealablebysonar()==true )
        {
            if (sqrt(((*p)->getX()-x)*((*p)->getX()-x) + ((*p)->getY()-y)*((*p)->getY()-y))<12.0)
                (*p)->setVisible(true);
        }
    }
}
void StudentWorld::dropnuggets(int x, int y)
{
    actor = new GoldNugget(this, x,y,pickablebyprotesters,true);
    l_actors.push_back(actor);
}

bool StudentWorld::nuggetpickbyprotesters(int x, int y)
{
    list<Actor*>::iterator p= l_actors.begin();
    for (;p!=l_actors.end(); p++)
    {
        if ((*p)->canPickThingsUp()==true )
        {
            if (sqrt(((*p)->getX()-x)*((*p)->getX()-x) + ((*p)->getY()-y)*((*p)->getY()-y))<=3.0)
            {
                (*p)->addGold();
                return true;
            }
        }
    }
    return false;
}
bool StudentWorld:: squirtannoyprotesters(int x, int y) //return true if there are protesters within the range of squirt and protesters get annoyed
{
    bool b=false;
    list<Actor*>::iterator p= l_actors.begin();
    for (;p!=l_actors.end(); p++)
    {
        if ((*p)->annoyable()==true )
        {
            if (sqrt(((*p)->getX()-x)*((*p)->getX()-x) + ((*p)->getY()-y)*((*p)->getY()-y))<=3.0)
            {
                (*p)->decreaseHealth(2);
                //if ((*p)->gethealth()>0) playSound(SOUND_PROTESTER_ANNOYED);
                b=true;
            }
        }
    }
    return b;
}
 bool StudentWorld::checkboulderinanarea(int x1, int x2, int y1, int y2) //does not include the boundary of the area
{
    list<Actor*>::iterator p= l_actors.begin();
    for (;p!=l_actors.end(); p++)
    {
        if ((*p)->canActorsPassThroughMe()==false )
        {
            if (((*p)->getX()>x1) && ((*p)->getX()<x2) && ((*p)->getY()>y1) && ((*p)->getY()<y2))
                return true;
        }
    }
    return false;
}
bool StudentWorld::checkiceinarea(int x1, int x2,int y1,int y2) //
{
    for (int i=0;i<(x2-x1);i++)
        for (int j=0; j<(y2-y1);j++)
        {
            if (ice_array[x1+i][y1+j]!=nullptr)
            return true;
        }
    return false;
}
void StudentWorld::bonkprotesters(int x, int y)
{
    //bool b=false;
    list<Actor*>::iterator p= l_actors.begin();
    for (;p!=l_actors.end(); p++)
    {
        if ((*p)->annoyable()==true && (*p)->getState()!=falling)
        {
            if (sqrt(((*p)->getX()-x)*((*p)->getX()-x) + ((*p)->getY()-y)*((*p)->getY()-y))<=3.0)
            {
                (*p)->decreaseHealth(100);
            }
        }
    }
    
    if ((iceman_p->getX()-x)*(iceman_p->getX()-x) +(iceman_p->getY()-y)*(iceman_p->getY()-y)<=3.0)
        iceman_p->decreaseHealth(100);
    
    
    //return b;
}
bool StudentWorld::randompostionclear(int x, int y)
{
    
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
        {
            if (ice_array[x+i][y+j]!=nullptr)
                return false;
        }
    return true;
}

void StudentWorld:: addProtester()
{
    int poh=getLevel()*10+30;
    int probabilityOfHardcore=min(90,poh);
    int r=rand()%100+1;
    if (r<probabilityOfHardcore)
    {
        HardcoreProtester *hardpro= new HardcoreProtester(this);
        l_actors.push_back(hardpro);
        num_protesters++;}
    else
    {
        RegProtester *regpro= new RegProtester(this);
        l_actors.push_back(regpro);
        num_protesters++;
    }
    Protesterticks=0;
}



// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
