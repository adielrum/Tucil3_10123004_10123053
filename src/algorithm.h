#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <bits/stdc++.h>
#include "piece.h"
#include "move.h"
#include "papan.h"
#include "state.h"

// Helpers
std::string gridToString(const std::vector<std::vector<char>>& grid);

// Algorithms
std::vector<std::pair<Piece, Move>> AStar(const State& initial_state);

#endif
