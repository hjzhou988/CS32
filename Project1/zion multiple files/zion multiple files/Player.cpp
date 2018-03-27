//
//  Player.cpp
//  zion multiple files
//
//  Created by Huajun Zhou on 7/2/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#include "Player.h"
#include "Arena.h"
#include <iostream>
using namespace std;


Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col; }


int Player::age() const
{
    return m_age;
}

string Player::takeComputerChosenTurn()
{
    //Get anena dimension
    int ncols = m_arena->cols();
    int nrows = m_arena->rows();
    
    //The situation when player is surrounded by robots.
    if (m_arena->nRobotsAt(m_row,m_col-1)!=0 &&
        m_arena->nRobotsAt(m_row-1,m_col)!=0 &&
        m_arena->nRobotsAt(m_row,m_col+1)!=0 &&
        m_arena->nRobotsAt(m_row+1,m_col)!=0)
    {
        if (m_arena->nRobotsAt(m_row,m_col-1)>0)
        {
            move(LEFT);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(m_row,m_col+1)>0)
        {
            move(RIGHT);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(m_row-1,m_col)>0)
        {
            move(UP);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(m_row+1,m_col)>0)
        {
            move(DOWN);
            return "Moved.";
        }
    }
    
    //The situation when robots were 2 blocks away in every direction
    if (m_arena->nRobotsAt(m_row,m_col-2)!=0 &&
        m_arena->nRobotsAt(m_row-2,m_col)!=0 &&
        m_arena->nRobotsAt(m_row,m_col+2)!=0 &&
        m_arena->nRobotsAt(m_row+2,m_col)!=0)
    {
        if (m_arena->nRobotsAt(m_row,m_col-2)>0)
        {
            if (shoot(LEFT)) return "Shot and hit!";
            else return "Shot and missed!";
        }
        if (m_arena->nRobotsAt(m_row,m_col+2)>0)
        {
            if (shoot(RIGHT)) return "Shot and hit!";
            else return "Shot and missed!";
        }
        if (m_arena->nRobotsAt(m_row-2,m_col)>0)
        {
            if (shoot(UP)) return "Shot and hit!";
            else return "Shot and missed!";
        }
        if (m_arena->nRobotsAt(m_row+2,m_col)>0)
        {
            if (shoot(DOWN)) return "Shot and hit!";
            else return "Shot and missed!";
        }
    }

    
    if (m_row==1 && m_col==1)// Player is in the top left corner
    {
        if (m_arena->nRobotsAt(1, 2)!=0 && m_arena->nRobotsAt(2,1)!=0)
        { if (shoot(DOWN)) return "Shot and hit!";
        else return "Shot and missed!";
        }
        if (m_arena->nRobotsAt(1, 2)!=0 && m_arena->nRobotsAt(2,1)==0)
        {
            move(DOWN);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(1, 2)==0 && m_arena->nRobotsAt(2,1)!=0)
        {
            move(RIGHT);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(1, 2)==0 && m_arena->nRobotsAt(2,1)==0)
        {
            if(m_arena->nRobotsAt(1, 3) && m_arena->nRobotsAt(3,1))
            {
                if (shoot(DOWN)) return "Shot and hit!";
                else return "Shot and missed!";
            }
            if(m_arena->nRobotsAt(1, 3)>0 && m_arena->nRobotsAt(3,1)==0)
            {
                if (shoot(RIGHT)) return "Shot and hit!";
                else return "Shot and missed!";
            }
            if(m_arena->nRobotsAt(1, 3)==0 && m_arena->nRobotsAt(3,1)>0)
            {
                if (shoot(DOWN)) return "Shot and hit!";
                else return "Shot and missed!";
                
            }
            
        }
    }
    if (m_row==1 && m_col==ncols) // Player is in the top right corner
    {
        if (m_arena->nRobotsAt(1, ncols-1)!=0 && m_arena->nRobotsAt(2,ncols)!=0)
        { if (shoot(DOWN)) return "Shot and hit!";
        else return "Shot and missed!";
        }
        if (m_arena->nRobotsAt(1, ncols-1)!=0 && m_arena->nRobotsAt(2,ncols)==0)
        {
            move(DOWN);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(1, ncols-1)==0 && m_arena->nRobotsAt(2,ncols)!=0)
        {
            move(LEFT);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(1, ncols-1)==0 && m_arena->nRobotsAt(2,ncols)==0)
        {
            if(m_arena->nRobotsAt(1, ncols-2) && m_arena->nRobotsAt(3,ncols))
            {
                if (shoot(DOWN)) return "Shot and hit!";
                else return "Shot and missed!";
            }
            if(m_arena->nRobotsAt(1, ncols-2)>0 && m_arena->nRobotsAt(3,ncols)==0)
            {
                if (shoot(LEFT)) return "Shot and hit!";
                else return "Shot and missed!";
            }
            if(m_arena->nRobotsAt(1, ncols-2)==0 && m_arena->nRobotsAt(3,ncols)>0)
            {
                if (shoot(DOWN)) return "Shot and hit!";
                else return "Shot and missed!";
                
            }
            
        }
    }
    if (m_row==nrows && m_col==1) // Player is in the bottom left corner
    {
        if (m_arena->nRobotsAt(nrows-1, 1)!=0 && m_arena->nRobotsAt(nrows,2)!=0)
        { if (shoot(UP)) return "Shot and hit!";
        else return "Shot and missed!";
        }
        if (m_arena->nRobotsAt(nrows-1, 1)!=0 && m_arena->nRobotsAt(nrows,2)==0)
        {
            move(RIGHT);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(nrows-1, 1)==0 && m_arena->nRobotsAt(nrows,2)!=0)
        {
            move(UP);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(nrows-1, 1)==0 && m_arena->nRobotsAt(nrows,2)==0)
        {
            if(m_arena->nRobotsAt(nrows-2, 1) && m_arena->nRobotsAt(nrows,3))
            {
                if (shoot(UP)) return "Shot and hit!";
                else return "Shot and missed!";
            }
            if(m_arena->nRobotsAt(nrows-2, 1)>0 && m_arena->nRobotsAt(nrows,3)==0)
            {
                if (shoot(UP)) return "Shot and hit!";
                else return "Shot and missed!";
            }
            if(m_arena->nRobotsAt(nrows-2, 1)==0 && m_arena->nRobotsAt(nrows,3)>0)
            {
                if (shoot(RIGHT)) return "Shot and hit!";
                else return "Shot and missed!";
                
            }
            
        }
    }
    if (m_row==nrows && m_col==ncols) // Player is in the bottom right corner
    {
        if (m_arena->nRobotsAt(nrows-1, ncols)!=0 && m_arena->nRobotsAt(nrows,ncols-1)!=0)
        { if (shoot(UP)) return "Shot and hit!";
        else return "Shot and missed!";
        }
        if (m_arena->nRobotsAt(nrows-1, ncols)!=0 && m_arena->nRobotsAt(nrows,ncols-1)==0)
        {
            move(LEFT);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(nrows-1, ncols)==0 && m_arena->nRobotsAt(nrows,ncols-1)!=0)
        {
            move(UP);
            return "Moved.";
        }
        if (m_arena->nRobotsAt(nrows-1, ncols)==0 && m_arena->nRobotsAt(nrows,ncols-1)==0)
        {
            if(m_arena->nRobotsAt(nrows-2, ncols) && m_arena->nRobotsAt(nrows,ncols-2))
            {
                if (shoot(UP)) return "Shot and hit!";
                else return "Shot and missed!";
            }
            if(m_arena->nRobotsAt(nrows-2, ncols)>0 && m_arena->nRobotsAt(nrows,ncols-2)==0)
            {
                if (shoot(UP)) return "Shot and hit!";
                else return "Shot and missed!";
            }
            if(m_arena->nRobotsAt(nrows-2, ncols)==0 && m_arena->nRobotsAt(nrows,ncols-2)>0)
            {
                if (shoot(LEFT)) return "Shot and hit!";
                else return "Shot and missed!";
                
            }
            
        }
    }
    
    // Saw robots 2 to 5 blocks away and going to shoot.
    bool Robotinrange=false;
    for (int i=2; i<=MAXSHOTLEN; i++)
        if(m_arena->nRobotsAt(m_row,m_col-i)>0)
        {
            Robotinrange=true;
            break;
        }
    if (Robotinrange)
    {
        if (shoot(LEFT)) return "Shot and hit!";
        else return "Shot and missed!";
    }
    for (int i=2; i<=MAXSHOTLEN; i++)
        if(m_arena->nRobotsAt(m_row,m_col+i)>0)
        {
            Robotinrange=true;
            break;
        }
    if (Robotinrange)
    {
        if (shoot(RIGHT)) return "Shot and hit!";
        else return "Shot and missed!";
    }
    for (int i=2; i<=MAXSHOTLEN; i++)
        if(m_arena->nRobotsAt(m_row+i,m_col)>0)
        {
            Robotinrange=true;
            break;
        }
    if (Robotinrange)
    {
        if (shoot(DOWN)) return "Shot and hit!";
        else return "Shot and missed!";
    }
    for (int i=2; i<=MAXSHOTLEN; i++)
        if(m_arena->nRobotsAt(m_row-i,m_col)>0)
        {
            Robotinrange=true;
            break;
        }
    if (Robotinrange)
    {
        if (shoot(UP)) return "Shot and hit!";
        else return "Shot and missed!";
    }

    
    stand();
    return "Stood";// Your replacement implementation should do something intelligent
    // and return a string that describes what happened.  When you've
    // decided what action to take, take it by calling move, shoot, or stand.
    // This function must return one of the following four strings:
    //     "Moved."
    //     "Shot and hit!"
    //     "Shot and missed!"
    //     "Stood."
    
    // Here's one possible strategy:
    //   If moving in some direction would put me in less immediate danger
    //     than standing, then move in that direction.
    //   else shoot in the direction of the nearest robot I can hit.
    
    // A more aggressive strategy is possible, where you hunt down robots.
}

void Player::stand()
{
    m_age++;
    m_arena->history().record(row(),col());
}

void Player::move(int dir)
{
    
    m_age++;
        switch (dir)
    {
        case UP:// TODO:  Move the player up one row if possible.
            if (m_row==1) break;
            m_row--;
            break;
        case DOWN:
            if (m_row== m_arena->rows()) break;
            m_row++;
            break;
        case LEFT:
            if (m_col==1) break;
            m_col--;
            break;
        case RIGHT:
            if (m_col==m_arena->cols()) break;
            m_col++;
            break;
    }
    m_arena->history().record(row(),col());

    
}

bool Player::shoot(int dir)
{
    m_age++;
    m_arena->history().record(row(),col());
    if (rand() % 3 == 0)  // miss with 1/3 probability
        return false;
    
    if (rand()%3 != 0)
        switch (dir)
    {
        case UP:
            for (int i=1; i<=MAXSHOTLEN; i++)
                if ( m_arena->nRobotsAt(m_row-i, m_col))
                {
                    m_arena->damageRobotAt(m_row-i,m_col);
                    return true;
                }
            return false;
        case DOWN:
            for (int i=1; i<=MAXSHOTLEN; i++)
                if ( m_arena->nRobotsAt(m_row+i, m_col))
                {
                    m_arena->damageRobotAt(m_row+i,m_col);
                    return true;
                }
            return false;
        case LEFT:
            for (int i=1; i<=MAXSHOTLEN; i++)
                if ( m_arena->nRobotsAt(m_row, m_col-i))
                {
                    m_arena->damageRobotAt(m_row,m_col-i);
                    return true;
                }
            return false;
        case RIGHT:
            for (int i=1; i<=MAXSHOTLEN; i++)
                if ( m_arena->nRobotsAt(m_row, m_col+i))
                {
                    m_arena->damageRobotAt(m_row,m_col+i);
                    return true;
                }
            return false;
    }
    
    // TODO:  Damage the nearest robot in direction dir, returning
    // true if a robot is hit and damaged, false if not hit.
    return false;// This implementation compiles, but is incorrect.
}

bool Player::isDead() const
{
    if (m_arena->nRobotsAt(m_row,m_col) || m_dead==true)// if m_col == anyrobots' coordiates, return true and setDead?, else return false
        return true ;
    return false;
}

void Player::setDead()
{
    m_dead = true;
}
