#include "papan.h"

// Konstruktor
Papan::Papan(vector<string> _grid, int _rows, int _cols)
    : rows(_rows + 2), cols(_cols + 2), exit_x(0), exit_y(0) {
    cout << "---------------Awal-------------------" << endl;
    for(auto s : _grid){
        cout << s << endl; 
    }
    cout << "---------------Awal-------------------" << endl;
    // Initialize grid with sentinels 
    grid.resize(rows, vector<char>(cols, '*'));
    
    // Find position of exit cell (K)
    int raw_exit_x = -1, raw_exit_y = -1;
    int row_delete = -1;
    for (int i = 0; i < _grid.size(); i++) {
        size_t pos = _grid[i].find('K');
        // cout << pos << endl;
        if (pos != std::string::npos) {
            // pos menyatakan indeks k;
            raw_exit_x = i;  
            raw_exit_y = static_cast<int>(pos); 
            
            _grid[i].erase(pos, 1);  
            cout << _grid[i] << endl;

            if ((i == 0 || i == _rows + 1)) {
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
    if (row_delete != -1) {
        _grid.erase(_grid.begin() + row_delete);
    }
    cout << "---------------Hasil-------------------" << endl;
    for(auto s : _grid){
        cout << s << endl; 
    }
    cout << "---------------Hasil-------------------" << endl;
    // rows = _grid.size();
    // cols = _grid[0].size();

    // Copy grid to papan
    for(int i = 0; i < _rows; i++) {
        for(int j = 0; j < _cols; j++) {
            grid[i+1][j+1] = _grid[i][j];
        }
    }

    for(auto s : grid) {
        for(auto a : s) {
            cout << a << " ";
        }
        cout << endl;
    }

    // Exit at first row
    // if (raw_exit_x == 0) { 
    //     for (int i = 0; i < _grid.size(); i++) {
    //         for (int j = 0; j < _grid[i].size(); j++) {
    //             if (j == raw_exit_y) {
    //                 if (i == 0) continue;
    //                 grid[i][j + 1] = _grid[i][j];
    //             } else {
    //                 grid[i + 1][j + 1] = _grid[i][j];
    //             }
    //         }
    //     }
    // } 
    
    // // Exit at first column
    // else if (raw_exit_y == 0) { 
    //     for (int i = 0; i < _grid.size(); i++) {
    //         for (int j = 0; j < _grid[i].size(); j++) {
    //             if (i == raw_exit_x && j == 0) continue;
    //             if (i == raw_exit_x) {
    //                 grid[i + 1][j] = _grid[i][j];
    //             } else {
    //                 grid[i + 1][j + 1] = _grid[i][j];
    //             }
    //         }
    //     }
    //     exit_y = raw_exit_y;
    //     exit_x = raw_exit_x + 1;
    // } 
    
    // // Exit at other edges
    // else if (_grid.size() != _rows) {
    //     for (int i = 0; i < _grid.size() - 1; i++) {
    //         for (int j = 0; j < _grid[i].size(); j++) {
    //             grid[i + 1][j + 1] = _grid[i][j];
    //         }
    //     }
    //     exit_x = raw_exit_x + 1;
    //     exit_y = raw_exit_y + 1;
    // } 
    
    // else { 
    //     for (int i = 0; i < _grid.size(); i++) {
    //         for (int j = 0; j < _grid[i].size(); j++) {
    //             grid[i + 1][j + 1] = _grid[i][j];
    //         }
    //     }
    //     exit_x = raw_exit_x + 1;
    //     exit_y = raw_exit_y + 1;
    // }

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
