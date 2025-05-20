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

State State::applyMove(const pair<Piece, Move>& action) const {
   Piece target = action.first;
   Move move     = action.second;

   // Copy current state
   vector<pair<Piece, Move>> current_moves = this->list_moves;
   vector<Piece> new_pieces = this->pieces;
   Papan new_papan = this->papan;

   // Find & update the moved piece
   for (Piece& p : new_pieces) {
      if (p.name != target.name) continue;

      // 1) Erase from its old cells
      if (p.ori == 0) {  // horizontal
         for (int j = 0; j < p.len; ++j)
            new_papan.grid[p.pos_x][p.pos_y + j] = '.';
      } else {          // vertical
         for (int i = 0; i < p.len; ++i)
            new_papan.grid[p.pos_x + i][p.pos_y] = '.';
      }

      // 2) Update its coords
      if      (move.arah == "kiri")  p.pos_y -= move.dist;
      else if (move.arah == "kanan") p.pos_y += move.dist;
      else if (move.arah == "atas")  p.pos_x -= move.dist;
      else if (move.arah == "bawah") p.pos_x += move.dist;

   // 3) Detect exit move (only for 'P'), using original piece to find exit side:
   bool isExitMove = false;
   if (p.name == 'P') {
      // original piece before move:
      const Piece& orig = action.first;
      int o_head_x = orig.pos_x;
      int o_head_y = orig.pos_y;
      int o_tail_x = orig.pos_x + (orig.ori == 1 ? orig.len - 1 : 0);
      int o_tail_y = orig.pos_y + (orig.ori == 0 ? orig.len - 1 : 0);

      int ex = new_papan.exit_x;
      int ey = new_papan.exit_y;

      // decide exit direction once:
      enum { RIGHT, LEFT, DOWN, UP, NONE } exitDir = NONE;
      if (orig.ori == 0 && o_head_x == ex) {
         if (ey > o_tail_y) exitDir = RIGHT;
         else if (ey < o_head_y) exitDir = LEFT;
      }
      else if (orig.ori == 1 && o_head_y == ey) {
         if (ex > o_tail_x) exitDir = DOWN;
         else if (ex < o_head_x) exitDir = UP;
      }

      // now see if this move matches that direction & crosses the exit
      int head_x = p.pos_x;
      int head_y = p.pos_y;
      int tail_x = p.pos_x + (p.ori == 1 ? p.len - 1 : 0);
      int tail_y = p.pos_y + (p.ori == 0 ? p.len - 1 : 0);

      switch (exitDir) {
         case RIGHT:
         if (move.arah == "kanan" && tail_y >= ey) isExitMove = true;
         break;
         case LEFT:
         if (move.arah == "kiri"  && head_y <= ey) isExitMove = true;
         break;
         case DOWN:
         if (move.arah == "bawah" && tail_x >= ex) isExitMove = true;
         break;
         case UP:
         if (move.arah == "atas"  && head_x <= ex) isExitMove = true;
         break;
         default:
         break;
      }
   }

      // 4) Redraw unless it's the exit move
      if (!isExitMove) {
         if (p.ori == 0) {
            for (int j = 0; j < p.len; ++j)
               new_papan.grid[p.pos_x][p.pos_y + j] = p.name;
         } else {
            for (int i = 0; i < p.len; ++i)
               new_papan.grid[p.pos_x + i][p.pos_y] = p.name;
         }
      }

      // 5) Record the move
      current_moves.emplace_back(p, move);
      break;
   }

   return State(new_papan, current_moves, new_pieces, this->depth + 1);
}

// Heuristic 1 (Admissible): Calculate blocking cars
int State::computeBlocks() const {

   vector<vector<char>> grid = papan.grid;
   pair<int, int> k_pos = {papan.exit_x, papan.exit_y};
   int blocked_cells = 0;

   // Find primary piece 
   for (Piece p : pieces) {
      if (p.name == 'P') {

         // Horizontal primary piece
         if (p.ori == 0) {

            // Coordinate bounds 
            int i_0 = min(p.pos_y, k_pos.second);
            int i_a = max(p.pos_y, k_pos.second);
            
            // Count blocked cells between primary cell and exit cell 
            for (int i = i_0; i < i_a; i++) {
               char c = grid[p.pos_x][i];
               if (c != 'K' && c != 'P' && c != '.') {
                  ++blocked_cells;
               }
            }
            
         } 
         
         // Vertical primary piece
         else {

            // Coordinate bounds 
            int i_0 = min(p.pos_x, k_pos.first);
            int i_a = max(p.pos_x, k_pos.first);

            // Count blocked cells between primary cell and exit cell 
            for (int i = i_0; i < i_a; i++) {
               char c = grid[i][p.pos_y];
               if (c != 'K' && c != 'P' && c != '.') {
                  ++blocked_cells;
               }
            }
         }

         break; 

      }
   }

   return blocked_cells;

}

// Heuristic 2 (Non-admissible): Compute distance between primary piece and exit cell
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
