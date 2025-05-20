#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <bits/stdc++.h>
#include "piece.h"
#include "move.h"
#include "papan.h"
#include "state.h"

// Helpers
string gridToString(const vector<vector<char>>& grid);

// Algorithms
vector<pair<Piece, Move>> solveBoard(const State& initial_state);

#endif
