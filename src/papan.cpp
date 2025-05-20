#include "papan.h"

// Konstruktor
Papan::Papan(vector<string> _grid, int _rows, int _cols)
    : rows(_rows + 2), cols(_cols + 2), exit_x(0), exit_y(0) {
    
    // Initialize grid with sentinels 
    grid.resize(rows, vector<char>(cols, '*'));
    
    // Find position of exit cell (K)
    int raw_exit_x = -1, raw_exit_y = -1;
    int row_delete = -1;

    for (int i = 0; i < _grid.size(); i++) {

        // Locate exit cell 
        size_t pos = _grid[i].find('K');
        if (pos != std::string::npos) {

            // Save exit cell coordinates
            raw_exit_x = i;  
            raw_exit_y = static_cast<int>(pos); 
            
            // Erase exit cell 
            _grid[i].erase(pos, 1);  
            if (i == 0 || i == _rows + 1) {
                row_delete = i;
            }

            // Add padding 
            if (i == 0) {
                exit_x = raw_exit_x;
                exit_y = raw_exit_y + 1;
            } else if (pos == 0 ) {
                exit_x = raw_exit_x + 1;
                exit_y = raw_exit_y;
            } else {
                exit_x = raw_exit_x + 1;
                exit_y = raw_exit_y + 1;
            }

            break;
        }
    }

    // Remove exit row if necessary
    if (row_delete != -1) {
        _grid.erase(_grid.begin() + row_delete);
    }

    // Copy grid to papan
    for(int i = 0; i < _rows; i++) {
        for(int j = 0; j < _cols; j++) {
            grid[i+1][j+1] = _grid[i][j];
        }
    }

    // Open exit cell
    grid[exit_x][exit_y] = 'K';
}

// Display papan
void Papan::printGrid(char s) {
    for (const auto& row : grid) {
        for (char c : row) {
            if (c == s) {
                cout << "\033[43m" << c << "\033[0m ";
            } else if (c == 'P') {
                cout << "\033[1;31m" << c << "\033[0m ";
            } else if (c == 'K') {
                cout << "\033[1;32m" << c << "\033[0m ";
            } else if (c == '*') {
                cout << "\033[1;37m" << c << "\033[0m ";
            } else {
                cout << c << "\033[0m ";
            }
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
