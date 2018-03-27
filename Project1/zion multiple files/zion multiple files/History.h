//
//  History.hpp
//  zion multiple files
//
//  Created by Huajun Zhou on 7/2/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#ifndef History_h
#define History_h


#include "globals.h"


class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int grid[MAXROWS][MAXCOLS];
    int m_row;
    int m_col;
    char nth_letter(int) const;
    
};


#endif /* History_hpp */
