//
//  History.cpp
//  zion multiple files
//
//  Created by Huajun Zhou on 7/2/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#include "History.h"
#include <iostream>
using namespace std;


History::History (int nRows, int nCols)
{
    m_row=nRows;
    m_col=nCols;
    for (int i=0; i<nRows; i++)
        for (int j=0; j<nCols; j++)
            grid[i][j]=0;
}

bool History::record(int r, int c)
{
    if (r>m_row || c>m_col) return false;
    else {
        grid[r-1][c-1]++;
        return true;
    }
}

void History::display() const
{
    clearScreen();
    for (int r = 0; r < m_row; r++)
    {
        for (int c = 0; c < m_col; c++)
            cout << nth_letter(grid[r][c]);
        cout << endl;
    }
    cout << endl;
}

char History::nth_letter(int n) const
{
if (n==0)
    return '.';
else if (n >= 1 && n <= 26)
        return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[n-1];
else if (n>26)
        return 'Z';
    else cout<< "Error in histroy recording!";
    exit(1);
}


