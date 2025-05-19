#ifndef MOVE_H
#define MOVE_H

#include <bits/stdc++.h>
using namespace std;

struct Move {
    string arah; // arah gerak: "atas", "bawah", "kanan", "kiri"
    int dist;    // jarak gerak 

    // Constructor
    Move(); 
    Move(string _arah, int _dist); 

    // Method
    void displayMove(); 
};

#endif
