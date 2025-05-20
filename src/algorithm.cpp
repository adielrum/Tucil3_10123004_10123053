#include <bits/stdc++.h>
#include "algorithm.h"

// Convert grid to string 
string gridToString(const vector<vector<char>>& grid) {
    string result;
    // Skip the sentinel border (first and last rows/columns)
    for (size_t i = 1; i < grid.size() - 1; i++) {
        for (size_t j = 1; j < grid[i].size() - 1; j++) {
            result.push_back(grid[i][j]);
        }
    }
    return result;
}

// A* Search Algorithm 
vector<pair<Piece, Move>> solveBoard(const State& initial_state) {
   int nodes_explored = 0;
   
   // Create priority queue 
   auto compare = [](const pair<double, State>& a, const pair<double, State>& b) {
      return a.first > b.first;  // Lower cost has higher priority
   };
   priority_queue<pair<double, State>, vector<pair<double, State>>, decltype(compare)> pq(compare);
   
   // Visited states
   unordered_set<string> visited;
   
   // Push initial state
   double initial_cost = 1.0 * initial_state.depth + initial_state.computeBlocks();
   pq.push({initial_cost, initial_state});
   
   // Loop through queue
   while (!pq.empty()) {

      // Get state with lowest cost
      State current = pq.top().second;
      double test = pq.top().first;
      pq.pop();
      
      // Skip state if already visited 
      string grid_str = gridToString(current.papan.grid);
      if (visited.count(grid_str)) continue;
      
      // Add to visited
      visited.insert(grid_str);
      nodes_explored++;
      
      // Find the primary piece (P)
      Piece* p_piece = nullptr;
      for (const Piece& p : current.pieces) {
         if (p.name == 'P') {
            p_piece = const_cast<Piece*>(&p);
            break;
         }
      }
      
      if (!p_piece) {
         cout << "Error: P piece not found!" << endl;
         continue;
      }
      
      // Check if goal state is reached (primary piece can exit)
      int px = p_piece->pos_x;
      int py = p_piece->pos_y;
      int ori = p_piece->ori;
      int len = p_piece->len;
      int tail; 
      bool can_exit = false;

      // Horizontal car
      if (ori == 0) {
         tail = py + len - 1;
         if (px == current.papan.exit_x) { // ensure same row

            // Exit is to the right
            if (current.papan.exit_y > tail) {
               can_exit = true;
               for (int c = tail + 1; c <= current.papan.exit_y; c++) {
                  char cell = current.papan.grid[px][c];
                  if (cell != '.' && cell != 'K') { can_exit = false; break; }
               }
            }
            
            // Exit is to the left
            else if (current.papan.exit_y < py) {
               can_exit = true;
               for (int c = py - 1; c >= current.papan.exit_y; c--) {
                  char cell = current.papan.grid[px][c];
                  if (cell != '.' && cell != 'K') { can_exit = false; break; }
               }
            }

         }
      }

      // Vertical car
      else {
         tail = px + p_piece->len - 1;
         if (py == current.papan.exit_y) { // ensure same column

               // Exit is below
               if (current.papan.exit_x > tail) {
                  can_exit = true;
                  for (int r = tail + 1; r <= current.papan.exit_x; r++) {
                     char cell = current.papan.grid[r][py];
                     if (cell != '.' && cell != 'K') { can_exit = false; break; }
                  }
               }

               // Exit is above
               else if (current.papan.exit_x < px) {
                  can_exit = true;
                  for (int r = px - 1; r >= current.papan.exit_x; r--) {
                     char cell = current.papan.grid[r][py];
                     if (cell != '.' && cell != 'K') { can_exit = false; break; }
                  }
               }

         }
      }
      
      // Goal state reached
      if (can_exit) {

         // 1. Figure out how far P must go to exit
         int tail = (p_piece->ori == 0)
               ? p_piece->pos_y + p_piece->len - 1
               : p_piece->pos_x + p_piece->len - 1;

         int dist_to_exit = 0;
         if (p_piece->ori == 0) {
               // horizontal
               if (current.papan.exit_y > tail) {
                  dist_to_exit = current.papan.exit_y - tail;
               } else {
                  // exit to the left
                  dist_to_exit = p_piece->pos_y - current.papan.exit_y;
               }
         } else {
               // vertical
               if (current.papan.exit_x > tail) {
                  dist_to_exit = current.papan.exit_x - tail;
               } else {
                  // exit above
                  dist_to_exit = p_piece->pos_x - current.papan.exit_x;
               }
         }

         // 2. Append the final move
         current.list_moves.emplace_back(*p_piece,
            Move( 
               (p_piece->ori == 0) 
                  ? (current.papan.exit_y > tail ? "kanan" : "kiri") 
                  : (current.papan.exit_x > tail ? "bawah" : "atas"),
               dist_to_exit + p_piece->len
            )
         );

         cout << "Solusi ditemukan!" << endl;
         cout << "Dikunjungi " << nodes_explored << " simpul dalam ";
         return current.list_moves;
      }
      
      // Generate all possible next states
      vector<pair<Piece, Move>> possible_moves = current.getAllPossibleMoves();
      for (const auto& move : possible_moves) {
         // Create new state by applying the move
         State next_state = current.applyMove(move);
         
         // Skip already visited states
         string next_grid_str = gridToString(next_state.papan.grid);
         if (visited.count(next_grid_str)) continue;
         
         // Calculate cost: f(n) = g(n) + h(n) 
         double cost = next_state.depth + next_state.computeBlocks();
         
         // Add to priority queue
         pq.push({cost, next_state});
      }
   }
   
   cout << "Solusi tidak ditemukan." << endl;
   cout << "Dikunjungi " << nodes_explored << " simpul dalam ";
   return {};  // No solution found
}