//
//  Arena.cpp
//  zion multiple files
//
//  Created by Huajun Zhou on 7/2/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#include "Arena.h"
#include <iostream>
using namespace std;
#include "Player.h"
#include "Robot.h"


Arena::Arena(int nRows, int nCols):m_history(nRows, nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRobots = 0;
}

Arena::~Arena()
{
    delete m_player;
    for (int k=0; k<m_nRobots; k++)
        delete m_robots[k];
    // TODO:  Delete the player and all remaining dynamically allocated robots.
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::robotCount() const
{
    return m_nRobots;
}

int Arena::nRobotsAt(int r, int c) const
{
    if (r>m_rows||r<1||c>m_cols||c<1)  //if coodinates out of range, return -1
        return -1;
    int nra=0;
    for (int i=0; i<m_nRobots; i++)
    {   if (m_robots[i]->row()==r && m_robots[i]->col()==c) // Attention! maybe incorrect.
        nra++;
    }
    return nra;
}

void Arena::display(string msg) const
{
    // Position (row,col) in the arena coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';
    
    // Indicate each robot's position// TODO:  If one robot is at some grid point, set the char to 'R'.
    //        If it's 2 though 8, set it to '2' through '8'.
    //        For 9 or more, set it to '9'.
    
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
        /*{
         if (nRobotsAt(r,c)>=2 && nRobotsAt(r,c)<= 9)
         grid[r][c]='0'+ nRobotsAt(r,c);
         if (nRobotsAt(r,c)>9)
         grid[r][c]='9';
         if (nRobotsAt(r,c)==1)
         grid[r][c]='R';
         }
         */
            
        {
            int k=0;
            for (int i=0; i<m_nRobots;i++)
            {
                if (m_robots[i]->row()-1==r && m_robots[i]->col()-1==c)
                {
                    grid[r][c]='R';
                    k++;
                }
            }
            if (k>=2 && k<=9) grid[r][c]='0'+k;
            if (k>9) grid[r][c]='9';
        }
    
    
    // Indicate player's position
    if (m_player != nullptr)
    {
        // Set the char to '@', unless there's also a robot there,
        // in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }
    
    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
    
    // Write message, robot, and player info
    cout << endl;
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Arena::addRobot(int r, int c)
{
    if (m_nRobots == MAXROBOTS) return false; // If MAXROBOTS have already been added, return false.  Otherwise,
    m_robots[m_nRobots]= new Robot(this, r, c);
    
    m_nRobots++;// dynamically allocate a new robot at coordinates (r,c).  Save the
    return true;// pointer to the newly allocated robot and return true.
    
}

bool Arena::addPlayer(int r, int c)
{
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Dynamically allocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

void Arena::damageRobotAt(int r, int c)
{// TODO:  Damage one robot at row r, column c if at least one is there.
    // If the robot does not survive the damage, destroy it.
    
    int nra= nRobotsAt(r,c);
    if (nra>=1)
    {
        int q = 0;// q is the sub number of the robot that will be damaged or destroyed.
        for (int i=0, k=0; i<=rand()% nra;k++)
            if (m_robots[k]->row()==r && m_robots[k]->col()==c)
            {q=k; i++;}
        if (!(m_robots[q]->takeDamageAndLive()))
        {
            delete m_robots[q];
            m_robots[q]=m_robots[m_nRobots-1];
            m_robots[m_nRobots-1]=nullptr;
            m_nRobots--;
        }
    }
    
    else
    {
        //cout<< "No robots in the range!";
        return;
    }
}

bool Arena::moveRobots()
{
    for (int k = 0; k < m_nRobots; k++)
    {
        m_robots[k]->move();// TODO:  Have the k-th robot in the arena make one move.
        if (m_robots[k]->row()==m_player->row()&& m_robots[k]->col()==m_player->col())
            m_player->setDead();//        If that move results in that robot being in the same
        //        position as the player, the player dies.
    }
    
    // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}

History& Arena::history()
{
    return m_history;
}


