#ifndef PIECE_H
#define PIECE_H

#include <bits/stdc++.h>

struct Piece {
    int pos_x; // posisi x
    int pos_y; // posisi y
    int len;   // panjang 
    int ori;   // orientasi (0: horizontal, 1: vertikal)
    char name; 

    // Constructor
    Piece();
    Piece(int _pos_x, int _pos_y, int _len, int _ori, char _name);
    
    // Method
    void displayPiece();
};

#endif