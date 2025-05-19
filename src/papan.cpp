#include "papan.h"

// Konstruktor
Papan::Papan(const vector<string>& _grid, int _rows, int _cols)
    : rows(_rows + 2), cols(_cols + 2), exit_x(0), exit_y(0) {
    
    // Initialize grid with sentinels 
    grid.resize(rows, vector<char>(cols, '*'));
    
    // Find position of exit cell (K)
    int raw_exit_x = -1, raw_exit_y = -1;
    for (int i = 0; i < _grid.size(); i++) {
        for (int j = 0; j < _grid[i].length(); j++) {
            if (_grid[i][j] == 'K') {
                raw_exit_x = i;
                raw_exit_y = j;
            }
        }
    }

    // Exit at first row
    if (raw_exit_x == 0) { 
        for (int i = 0; i < _grid.size(); i++) {
            for (int j = 0; j < _grid[i].size(); j++) {
                if (j == raw_exit_y) {
                    if (i == 0) continue;
                    grid[i][j + 1] = _grid[i][j];
                } else {
                    grid[i + 1][j + 1] = _grid[i][j];
                }
            }
        }
    } 
    
    // Exit at first column
    else if (raw_exit_y == 0) { 
        for (int i = 0; i < _grid.size(); i++) {
            for (int j = 0; j < _grid[i].size(); j++) {
                if (i == raw_exit_x && j == 0) continue;
                if (i == raw_exit_x) {
                    grid[i + 1][j] = _grid[i][j];
                } else {
                    grid[i + 1][j + 1] = _grid[i][j];
                }
            }
        }
        exit_y = raw_exit_y;
        exit_x = raw_exit_x + 1;
    } 
    
    // Exit at other edges
    else { 
        for (int i = 0; i < _grid.size(); i++) {
            for (int j = 0; j < _grid[i].size(); j++) {
                grid[i + 1][j + 1] = _grid[i][j];
            }
        }
        exit_x = raw_exit_x + 1;
        exit_y = raw_exit_y + 1;
    }

    // Open exit cell
    grid[exit_x][exit_y] = '.';
}

// Display papan
void Papan::printGrid() {
    for (const auto& row : grid) {
        for (char c : row) {
            cout << c << ' ';
        }
        cout << '\n';
    }
}

// Get all pieces from papan 
vector<Piece> Papan::extractPieces() {
    unordered_set<char> visited;
    vector<Piece> pieces;

    // Loop through entire grid
    int n = grid.size(), m = grid[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char ch = grid[i][j];

            // Skip empty or sentinel cells and visited pieces
            if (ch == '.' || ch == '*' || ch == 'K' || visited.count(ch)) continue;

            int len = 1, ori = -1;

            // Horizontal piece
            if (j + 1 < m && grid[i][j + 1] == ch) {
                ori = 0;
                int jj = j + 1;
                while (jj < m && grid[i][jj] == ch) {
                    ++len; ++jj;
                }
            } 
            
            // Vertical piece
            else if (i + 1 < n && grid[i + 1][j] == ch) {
                ori = 1;
                int ii = i + 1;
                while (ii < n && grid[ii][j] == ch) {
                    ++len; ++ii;
                }
            }

            // Add piece to list and mark as visited
            if (ori != -1) {
                pieces.emplace_back(i, j, len, ori, ch);
                visited.insert(ch);
            }
        }
    }

    return pieces;
}
