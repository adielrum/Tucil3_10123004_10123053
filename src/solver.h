#ifndef SOLVER_H
#define SOLVER_H

#include <bits/stdc++.h>
#include "piece.h"
#include "move.h"
#include "papan.h"
#include "state.h"

struct Solver {
   int type; // (0: UCS, 1: Greedy, 2: A*)
   
   // Konstruktor
   Solver(int _type);

   // Helper
   string gridToString(const vector<vector<char>>& grid);
   pair<int, int> toggleType(int type);

   // Metoda
   vector<pair<Piece, Move>> solveBoard(const State& initial_state);

};

#endif
