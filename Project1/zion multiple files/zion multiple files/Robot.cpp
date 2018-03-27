//
//  Robot.cpp
//  zion multiple files
//
//  Created by Huajun Zhou on 7/2/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#include "Robot.h"
#include <iostream>
using namespace std;
#include "Arena.h"

Robot::Robot(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A robot must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Robot created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    blood=2;
}

int Robot::row() const
{
    return m_row;
}

int Robot::col() const
{
    
    return m_col;
}

void Robot::move()
{
    // Attempt to move in a random direction; if we can't move, don't move
    switch (rand() % 4)
    {
        case UP:
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
}

bool Robot::takeDamageAndLive()
{
    if (blood ==2) { blood--; return true;}
    return false;
    // TODO:  If the robot has been hit once before, return false (since a
    // second hit kills a robot).  Otherwise, return true (since the robot
    // survived the damage).
}
