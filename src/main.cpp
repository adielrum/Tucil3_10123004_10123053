#include <bits/stdc++.h>
using namespace std;

int N, M, num_piece;

struct Piece {
    int pos_x; // posisi x
    int pos_y; // posisi y
    int len; // panjang
    int ori; // orientasi: 0 horizontal, 1 vertical
    char name;

    Piece() : name(' '), pos_x(0), pos_y(0), len(0), ori(0) {}

    // init mobil    
    Piece(int _pos_x, int _pos_y, int _len, int _ori, char _name)
        : pos_x(_pos_x), pos_y(_pos_y), len(_len), ori(_ori), name(_name) {}

    void displayPiece(){
        cout << "huruf: " << name << " baris: " << pos_x << " kolom: " << pos_y << " panjang: " << len << " orientasi: " << ((ori == 0) ? "horizontal" : "vertical") << endl;
    } 
};

struct Move {
    string arah; // "atas", "bawah", "kanan", "kiri"
    int dist; // berapa banyak

    Move() : arah(" "), dist(0) {}

    // init Move
    Move(string _arah, int _dist)
        : arah(_arah), dist(_dist) {}

    void displayMove(){
        cout << "arah: " << arah << " sebanyak " << dist << endl;
    }
};

struct Papan {
    int rows = 0;
    int cols = 0;
    int exit_x = 0;
    int exit_y = 0;
    vector<vector<char>> grid;
    
    // init papan
    Papan(const vector<string>& _grid, int _rows, int _cols) {
        rows = _rows + 2;
        cols = _cols + 2;

        // Initialize grid with sentinels
        grid.resize(rows, vector<char>(cols, '*'));

        // Variables to hold raw K position
        int raw_exit_x = -1, raw_exit_y = -1;

        // First pass: detect and remove 'K' from _grid clone
        for (int i = 0; i < _grid.size(); i++) {
            for (int j = 0; j < _grid[i].length(); j++) {
                if (_grid[i][j] == 'K') {
                    raw_exit_x = i;
                    raw_exit_y = j;
                }
            }
        }

        if (raw_exit_x == 0) {
            for (int i = 0; i < _grid.size(); i++) {
                for (int j = 0; j < _grid[i].size(); j++) {
                    if (j == raw_exit_y) {
                        if (i == 0) {
                            continue;
                        } else {
                            grid[i][j+1] = _grid[i][j];
                        }
                    } else {
                        grid[i+1][j+1] = _grid[i][j];
                    }
                }
            }
        } else if (raw_exit_y == 0) {
            for (int i = 0; i < _grid.size(); i++) {
                for (int j = 0; j < _grid[i].size(); j++) {
                    if (i == raw_exit_x) {
                        if (j == 0 ) {
                            continue;
                        } else {
                            grid[i+1][j] = _grid[i][j];
                        }
                    } else {
                        grid[i+1][j+1] = _grid[i][j];
                    }
                }
            }
            exit_y = raw_exit_y;
            exit_x = raw_exit_x + 1;
        } else {
            for(int i = 0; i < _grid.size(); i++) {
                for(int j = 0; j < _grid[i].size(); j++) {
                    grid[i+1][j+1] = _grid[i][j];
                }
            }
            exit_x = raw_exit_x + 1;
            exit_y = raw_exit_y + 1;
        }
        grid[exit_x][exit_y] = '.';
    }

    
    // Fungsi untuk output grid
    void printGrid() {
        for(unsigned i = 0; i < grid.size(); i++) {
            for(unsigned j = 0; j < grid[i].size(); j++) {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
    }

    vector<Piece> extractPieces() {
        unordered_set<char> visited;
        vector<Piece> pieces;

        int n = grid.size();
        int m = grid[0].size();

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                char ch = grid[i][j];

                if (ch == '.' || ch == '*' || ch == 'K' || visited.count(ch)) continue;

                // Try to determine orientation
                int len = 1;
                int ori = -1;

                // Check right (horizontal)
                if (j + 1 < m && grid[i][j + 1] == ch) {
                    ori = 0; // horizontal
                    int jj = j + 1;
                    while (jj < m && grid[i][jj] == ch) {
                        ++len;
                        ++jj;
                    }
                }
                // Check down (vertical)
                else if (i + 1 < n && grid[i + 1][j] == ch) {
                    ori = 1; // vertical
                    int ii = i + 1;
                    while (ii < n && grid[ii][j] == ch) {
                        ++len;
                        ++ii;
                    }
                }

                if (ori != -1) {
                    pieces.emplace_back(i, j, len, ori, grid[i][j]);
                    visited.insert(ch); // mark this piece as processed
                }
            }
        }

        return pieces;
    } 

};

struct State {
    Papan papan;
    vector<Piece> pieces;
    vector<pair<Piece,Move>> list_moves;
    int depth, row, col;

    // cari all possible moves
    // Konstruktor
    State(const Papan& _papan, const vector<pair<Piece,Move>> _list_moves, const vector<Piece>& _pieces, int _depth)
        : papan(_papan), list_moves(_list_moves), pieces(_pieces), depth(_depth), row(_papan.rows), col(_papan.cols) {}

    vector<pair<Piece, Move>> getAllPossibleMoves() {
        vector<pair<Piece, Move>> moves;

        for (Piece piece : pieces) {
            int x = piece.pos_x;
            int y = piece.pos_y;

            if (piece.ori == 0) { // Horizontal
                // Move left
                int steps = 1;
                while (papan.grid[x][y - steps] == '.') {
                    // cout << piece.name << " gerak sebanyak " << steps << " ke kiri" << endl;
                    moves.emplace_back(piece, Move("kiri", steps));
                    steps++;
                }

                // Move right
                steps = 1;
                int tail = y + piece.len -1;
                while (papan.grid[x][tail + steps] == '.') {
                    // cout << piece.name << " gerak sebanyak " << steps << " ke kanan" << endl;
                    moves.emplace_back(piece, Move("kanan", steps));
                    steps++;
                }
            } else { // Vertical
                // Move up
                int steps = 1;
                while (papan.grid[x - steps][y] == '.') {
                    // cout << piece.name << " gerak sebanyak " << steps << " ke atas" << endl;
                    moves.emplace_back(piece, Move("atas", steps));
                    steps++;
                }

                // Move down
                steps = 1;
                int tail = x + piece.len - 1;
                while (papan.grid[tail + steps][y] == '.') {
                    // cout << piece.name << " gerak sebanyak " << steps << " ke bawah" << endl;
                    moves.emplace_back(piece, Move("bawah", steps));
                    steps++;
                }
            }
        }

        return moves;
    }

    void printPieces() {
        for (const auto& p : pieces) {
            cout << "Piece " << p.name << " at (" << p.pos_x << ", " << p.pos_y << "), len=" << p.len << ", ori=" << (p.ori == 0 ? "horizontal" : "vertical") << endl;
        }
    }

    void printAllPossibleMoves() {
        vector<pair<Piece, Move>> moves = getAllPossibleMoves();
        for(auto a : moves) {
            a.first.displayPiece();
            a.second.displayMove();
        }
    }

    void printAllMoves() {
        for(auto a : list_moves) {
            a.first.displayPiece();
            a.second.displayMove();
        }
    }

    double computeEuclideanDistance() const {
        vector<vector<char>> grid = papan.grid;
        vector<pair<int, int>> p_positions;
        pair<int, int> k_position = {-1, -1};

        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] == 'P') {
                    p_positions.push_back({i, j});
                } else if (grid[i][j] == 'K') {
                    k_position = {i, j};
                }
            }
        }

        // Compute center of P (average of positions)
        double px = 0, py = 0;
        for (const auto& pos : p_positions) {
            px += pos.first;
            py += pos.second;
        }
        px /= p_positions.size();
        py /= p_positions.size();

        // K's position
        double kx = k_position.first;
        double ky = k_position.second;

        // Euclidean distance
        double dx = px - kx;
        double dy = py - ky;
        return sqrt(dx * dx + dy * dy);
    }

    double computeManhattanDistance() {
        vector<vector<char>> grid = papan.grid;
        vector<pair<int, int>> p_positions;
        pair<int, int> k_position = {-1, -1};

        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] == 'P') {
                    p_positions.push_back({i, j});
                } else if (grid[i][j] == 'K') {
                    k_position = {i, j};
                }
            }
        }

        // Compute center of P (average of positions, rounded to nearest int)
        double px = 0, py = 0;
        for (const auto& pos : p_positions) {
            px += pos.first;
            py += pos.second;
        }
        px /= p_positions.size();
        py /= p_positions.size();
        int center_px = round(px);
        int center_py = round(py);

        // Manhattan distance
        return abs(center_px - k_position.first) + abs(center_py - k_position.second);
    }
};

State applyMoveToState(const State& current, const pair<Piece, Move>& action) {
    Piece target = action.first; // piece
    Move move = action.second; // Move
    vector<pair<Piece, Move>> current_moves = current.list_moves;

    // Copy semua piece
    vector<Piece> new_pieces = current.pieces;

    // Copy papan
    Papan new_papan = current.papan;

    // Temukan dan update piece
    for (Piece& p : new_pieces) {
        if (p.name == target.name) {
            // 1. Hapus posisi lama dari grid
            if (p.ori == 0) { // horizontal
                for (int j = 0; j < p.len; ++j) {
                    new_papan.grid[p.pos_x][p.pos_y + j] = '.';
                }
            } else { // vertical
                for (int i = 0; i < p.len; ++i) {
                    new_papan.grid[p.pos_x + i][p.pos_y] = '.';
                }
            }

            // 2. Update posisi piece
            if (move.arah == "kiri") {
                p.pos_y -= move.dist;
            } else if (move.arah == "kanan") {
                p.pos_y += move.dist;
            } else if (move.arah == "atas") {
                p.pos_x -= move.dist;
            } else if (move.arah == "bawah") {
                p.pos_x += move.dist;
            }

            // 3. Tulis posisi baru ke grid
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

    // Return state baru
    return State(new_papan, current_moves, new_pieces, current.depth + 1);
}


// Convert grid to string for visited set comparison
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

// A* search algorithm using Euclidean distance heuristic
vector<pair<Piece, Move>> AStarEuclidean(const State& initial_state) {
    // For debugging
    int nodes_explored = 0;
    
    // Priority queue for A* search
    auto compare = [](const pair<double, State>& a, const pair<double, State>& b) {
        return a.first > b.first;  // Lower f-value has higher priority
    };
    priority_queue<pair<double, State>, vector<pair<double, State>>, decltype(compare)> pq(compare);
    
    // Set to keep track of visited states (to avoid cycles)
    unordered_set<string> visited;
    
    // Push initial state
    double initial_f = 1.0*initial_state.depth + initial_state.computeEuclideanDistance();
    pq.push({initial_f, initial_state});
    
    // Main A* loop
    while (!pq.empty()) {
        // Get the state with lowest f-value
        State current = pq.top().second;
        pq.pop();
        
        // Convert grid to string for visited check
        string grid_str = gridToString(current.papan.grid);
        
        // Skip if already visited
        if (visited.count(grid_str)) continue;
        
        // Add to visited
        visited.insert(grid_str);
        nodes_explored++;
        
        // Every 1000 nodes, print progress
        if (nodes_explored % 1000 == 0) {
            cout << "Nodes explored: " << nodes_explored 
                 << ", Queue size: " << pq.size() 
                 << ", Current depth: " << current.depth << endl;
        }
        
        // Find the P piece
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
        
        // Check if goal state is reached (P piece can exit)
        // Now check for exit in any direction:
        int px = p_piece->pos_x;
        int py = p_piece->pos_y;
        int tail; 
        bool can_exit = false;

        // Horizontal car
        if (p_piece->ori == 0) {
            tail = py + p_piece->len - 1;
            // same row?
            if (px == current.papan.exit_x) {
                // exit is to the right
                if (current.papan.exit_y > tail) {
                    can_exit = true;
                    for (int c = tail + 1; c <= current.papan.exit_y; c++) {
                        char cell = current.papan.grid[px][c];
                        if (cell != '.' && cell != 'K') { can_exit = false; break; }
                    }
                }
                // exit is to the left
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
            // same column?
            if (py == current.papan.exit_y) {
                // exit is below
                if (current.papan.exit_x > tail) {
                    can_exit = true;
                    for (int r = tail + 1; r <= current.papan.exit_x; r++) {
                        char cell = current.papan.grid[r][py];
                        if (cell != '.' && cell != 'K') { can_exit = false; break; }
                    }
                }
                // exit is above
                else if (current.papan.exit_x < px) {
                    can_exit = true;
                    for (int r = px - 1; r >= current.papan.exit_x; r--) {
                        char cell = current.papan.grid[r][py];
                        if (cell != '.' && cell != 'K') { can_exit = false; break; }
                    }
                }
            }
        }

                
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
                Move( (p_piece->ori == 0)
                    ? (current.papan.exit_y > tail ? "kanan" : "kiri")
                    : (current.papan.exit_x > tail ? "bawah" : "atas"),
                    dist_to_exit + 1));

            // 3. (Optional) Update the grid so P actually leaves:
            //    Remove old P cells:
            if (p_piece->ori == 0) {
                for (int j = 0; j < p_piece->len; ++j)
                    current.papan.grid[p_piece->pos_x][p_piece->pos_y + j] = '.';
            } else {
                for (int i = 0; i < p_piece->len; ++i)
                    current.papan.grid[p_piece->pos_x + i][p_piece->pos_y] = '.';
            }
            //    (And you may choose to place it “beyond” the board, or leave it blank.)

            cout << "Solution found! Explored " << nodes_explored << " nodes." << endl;
            return current.list_moves;
        }
        
        // Generate all possible next states
        vector<pair<Piece, Move>> possible_moves = current.getAllPossibleMoves();
        for (const auto& move : possible_moves) {
            // Create new state by applying the move
            State next_state = applyMoveToState(current, move);
            
            // Skip already visited states
            string next_grid_str = gridToString(next_state.papan.grid);
            if (visited.count(next_grid_str)) continue;
            
            // Calculate f-value = g(n) + h(n)
            // g(n) = depth of state (as requested)
            // h(n) = euclidean distance
            double f_value = next_state.depth + next_state.computeEuclideanDistance();
            
            // Add to priority queue
            pq.push({f_value, next_state});
        }
    }
    
    cout << "No solution found after exploring " << nodes_explored << " nodes." << endl;
    return {};  // No solution found
}

int main()
{
    cin >> N >> M >> num_piece;
    vector<string> temp_board;
    for(int i = 0; i < N; i++){
        string temp_row;
        cin >> temp_row;
        temp_board.push_back(temp_row);
    }
    Papan board = Papan(temp_board, N, M);
    // board.movePiece('I',4,4,3,0,-1);
    board.printGrid();
    vector<Piece> pieces = board.extractPieces();
    vector<pair<Piece, Move>> moves;
    State current_state(board, moves, pieces, 0);

    // cout << "\nRunning A* search with Euclidean distance heuristic..." << endl;
    vector<pair<Piece, Move>> solution = AStarEuclidean(current_state);

    if (!solution.empty()) {
        // cout << "\nSolution found in " << solution.size() << " moves:" << endl;
        for (size_t i = 0; i < solution.size(); i++) {
            cout << "Move " << (i+1) << ": ";
            cout << "Piece " << solution[i].first.name << " moves ";
            cout << solution[i].second.arah << " by " << solution[i].second.dist << endl;
        }
    } else {
        cout << "\nNo solution found." << endl;
    }
    // // current_state.printPieces();
    // current_state.printAllPossibleMoves();
    // vector<pair<Piece,Move>> gerak = current_state.getAllPossibleMoves();
    // State state_baru = applyMoveToState(current_state, gerak[5]);
    // state_baru.papan.printGrid();
    // state_baru.printAllMoves();
    // cout << state_baru.computeManhattanDistance();
    // vector<pair<Piece, Move>> solution = AStarEuclidean(current_state);
    // for (auto& step : solution) {
    // step.first.displayPiece();
    // step.second.displayMove();
// }

}