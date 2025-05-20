#ifndef STATE_H
#define STATE_H

#include <bits/stdc++.h>
#include "papan.h" 
#include "move.h" 

using namespace std;

struct State {
   Papan papan;
   vector<Piece> pieces;
   vector<pair<Piece, Move>> list_moves;
   int depth;
   int row, col;

   // Konstruktor
   State(
      const Papan& _papan, 
      const vector<pair<Piece,Move>>& _list_moves, 
      const vector<Piece>& _pieces, 
      int _depth
   );

   // Metoda
   vector<pair<Piece, Move>> getAllPossibleMoves();
   void printPieces();
   void printAllPossibleMoves();
   void printAllMoves();
   State applyMove(const pair<Piece, Move>& action) const;
   int computeDistance() const; 
   int computeBlocks() const;
   int computeDoubleBlocks() const;
   double computeEuclideanDistance() const;

};

#endif