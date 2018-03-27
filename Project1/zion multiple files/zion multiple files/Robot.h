//
//  Robot.hpp
//  zion multiple files
//
//  Created by Huajun Zhou on 7/2/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#ifndef Robot_h
#define Robot_h


class Arena;

class Robot
{
public:
    // Constructor
    Robot(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool takeDamageAndLive();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int   blood;// TODO: You'll probably find that a robot object needs an additional
    // data member to support your implementation of the behavior affected
    // by being hit and taking damage.
};

#endif /* Robot_hpp */
