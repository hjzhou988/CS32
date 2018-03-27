//
//  Game.hpp
//  zion multiple files
//
//  Created by Huajun Zhou on 7/2/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#ifndef Game_h
#define Game_h

class Arena;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
};


#endif /* Game_hpp */
