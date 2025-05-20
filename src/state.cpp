#include "state.h"

// Constructor 
State::State(const Papan& _papan, const vector<pair<Piece, Move>>& _list_moves, const vector<Piece>& _pieces, int _depth)
   : papan(_papan), list_moves(_list_moves), pieces(_pieces), depth(_depth), row(_papan.rows), col(_papan.cols) {}

// Get all possible moves
vector<pair<Piece, Move>> State::getAllPossibleMoves() {
   vector<pair<Piece, Move>> moves;

   for (Piece& piece : pieces) {
      int x = piece.pos_x;
      int y = piece.pos_y;

      // Horizontal piece
      if (piece.ori == 0) { 

         // Attmpt to move left
         int steps = 1;
         while (papan.grid[x][y - steps] == '.') {
            moves.emplace_back(piece, Move("kiri", steps));
            steps++;
         }

         // Attempt to move right
         steps = 1;
         int tail = y + piece.len - 1;
         while (papan.grid[x][tail + steps] == '.') {
            moves.emplace_back(piece, Move("kanan", steps));
            steps++;
         }

      } 
      
      // Vertical piece
      else { 

         // Attempt to move up
         int steps = 1;
         while (papan.grid[x - steps][y] == '.') {
            moves.emplace_back(piece, Move("atas", steps));
            steps++;
         }

         // Attempt to move down
         steps = 1;
         int tail = x + piece.len - 1;
         while (papan.grid[tail + steps][y] == '.') {
            moves.emplace_back(piece, Move("bawah", steps));
            steps++;
         }
      }
   }

   return moves;
}

// Print pieces
void State::printPieces() {
   for (const auto& p : pieces) {
      cout << "Piece " << p.name << " at (" << p.pos_x << ", " << p.pos_y << "), len=" << p.len
           << ", ori=" << (p.ori == 0 ? "horizontal" : "vertical") << endl;
   }
}

// Print all possible moves
void State::printAllPossibleMoves() {
   vector<pair<Piece, Move>> moves = getAllPossibleMoves();
   for (auto& a : moves) {
      a.first.displayPiece();
      a.second.displayMove();
   }
}

// Print all moves made so far
void State::printAllMoves() {
   for (auto& a : list_moves) {
      a.first.displayPiece();
      a.second.displayMove();
   }
}

// Apply move to state of papan
State State::applyMove(const pair<Piece, Move>& action) const {
   Piece target = action.first; // piece
   Move move = action.second; // movement

   // Duplicate fields 
   vector<pair<Piece, Move>> current_moves = this->list_moves;
   vector<Piece> new_pieces = this->pieces;
   Papan new_papan = this->papan;

   // Find dan update piece
   for (Piece& p : new_pieces) {
      if (p.name == target.name) {

         // 1. Lift piece from papan
         if (p.ori == 0) { // horizontal
               for (int j = 0; j < p.len; ++j) {
                  new_papan.grid[p.pos_x][p.pos_y + j] = '.';
               }
         } else { // vertical
               for (int i = 0; i < p.len; ++i) {
                  new_papan.grid[p.pos_x + i][p.pos_y] = '.';
               }
         }

         // 2. Update piece position
         if (move.arah == "kiri") {
               p.pos_y -= move.dist;
         } else if (move.arah == "kanan") {
               p.pos_y += move.dist;
         } else if (move.arah == "atas") {
               p.pos_x -= move.dist;
         } else if (move.arah == "bawah") {
               p.pos_x += move.dist;
         }

         // 3. Replace piece on papan
         if (p.ori == 0) { // horizontal
               for (int j = 0; j < p.len; ++j) {
                  new_papan.grid[p.pos_x][p.pos_y + j] = p.name;
               }
         } else { // vertical
               for (int i = 0; i < p.len; ++i) {
                  new_papan.grid[p.pos_x + i][p.pos_y] = p.name;
               }
         }
         
         current_moves.push_back({p, action.second});

         break;
      }
   }

   // Return new state
   return State(new_papan, current_moves, new_pieces, this->depth + 1);
}

// Heuristic 1: Compute distance between primary piece and exit cell
int State::computeDistance() const {
   vector<vector<char>> grid = papan.grid;
   pair<int, int> p_edge;
   pair<int, int> k_pos = {papan.exit_x, papan.exit_y};

   // Find primary piece edge cell
   for (Piece p : pieces) {
      if (p.name == 'P') {

         // Bottom-right-most cell
         if (k_pos.first != 0 && k_pos.second != 0) {

            // Horizontal → right-most cell
            if (p.ori == 0) {
               p_edge = {p.pos_x, p.pos_y + p.len - 1};
            } 
            
            // Vertical → bottom-most cell
            else {
               p_edge = {p.pos_x + p.len - 1, p.pos_y};
            }

            break;
         }

          // Top-left-most cell
         p_edge = {p.pos_x, p.pos_y};
         break;

      }
   }

   // Return shortest distance
   return abs(p_edge.first - k_pos.first) + abs(p_edge.second - k_pos.second);
}

// Heuristic 2: Calculate blocking cars

int State::computeBlocks() const {
   return 1;
}

// Euclidean distance calculation
double State::computeEuclideanDistance() const {
   vector<vector<char>> grid = papan.grid;
   vector<pair<int, int>> p_positions;
   pair<int, int> k_position = {-1, -1};

   for (int i = 0; i < (int)grid.size(); i++) {
      for (int j = 0; j < (int)grid[i].size(); j++) {
         if (grid[i][j] == 'P') {
            p_positions.push_back({i, j});
         } else if (grid[i][j] == 'K') {
            k_position = {i, j};
         }
      }
   }

   double px = 0, py = 0;
   for (const auto& pos : p_positions) {
      px += pos.first;
      py += pos.second;
   }
   px /= p_positions.size();
   py /= p_positions.size();

   double kx = k_position.first;
   double ky = k_position.second;

   double dx = px - kx;
   double dy = py - ky;
   return sqrt(dx * dx + dy * dy);
}
