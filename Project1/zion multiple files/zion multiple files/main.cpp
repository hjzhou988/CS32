//
//  main.cpp
//  zion multiple files
//
//  Created by Huajun Zhou on 7/1/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#include <iostream>
#include "Robot.h"
#include "Arena.h"
#include "Player.h"
#include "History.h"
#include "Game.h"
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    // Initialize the random number generator.  (You don't need to
    // understand how this works.)
    srand(static_cast<unsigned int>(time(0)));
    
    // Create a game
    //Game g(3, 3, 2);
    Game g(15, 18, 80);
    //Game g(10,2,1);
    //Game g(10,2,1);
    
    // Play the game
    g.play();
}
