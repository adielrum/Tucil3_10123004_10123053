#ifndef PAPAN_H
#define PAPAN_H

#include <bits/stdc++.h>
#include "Piece.h" 

using namespace std;

struct Papan {
    int rows;
    int cols;
    int exit_x;
    int exit_y;
    vector<vector<char>> grid;

    // Constructor
    Papan(vector<string> _grid, int _rows, int _cols);

    // Method
    void printGrid(char s = '-');
    vector<Piece> extractPieces();
};

#endif
