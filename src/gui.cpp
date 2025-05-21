#include <bits/stdc++.h>
#include "piece.h"
#include "move.h"
#include "papan.h"
#include "state.h"
#include "solver.h"

using namespace std;
using namespace chrono;

int N, M, num_piece;

int main()
{
        int N, M, num_piece;
        string algoritma_type = "2";
        cin >> algoritma_type;
        cin >> N >> M >> num_piece;

        bool ketemu_k = false;
        vector<string> temp_board;

        string temp_row;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        while(getline(cin, temp_row)){
            temp_board.push_back(temp_row);
        }

        Papan board = Papan(temp_board, N, M);
        if (!board.is_valid) {
            cout << "Papan tidak valid. Coba lagi.\n";
        }
        vector<Piece> pieces = board.extractPieces();
        vector<pair<Piece, Move>> moves;
        State current_state(board, moves, pieces, 0);

        // Solve papan 
        Solver boardSolver = Solver(stoi(algoritma_type));
        auto start = high_resolution_clock::now();
        vector<pair<Piece, Move>> solution = boardSolver.solveBoard(current_state);
        
        // Count time
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        double time = duration.count() / 1000.0;
        cout << fixed << setprecision(3) << time << " ms. \n" << endl;

        if (!solution.empty()) {
            for (size_t i = 0; i < solution.size(); i++) {
                cout << "Move " << (i+1) << ": ";
                cout << "Piece " << solution[i].first.name << " moves ";
                cout << solution[i].second.arah << " by " << solution[i].second.dist << endl;
            }
        } else {
            cout << "\nNo solution found." << endl;
        }
        cout << "\n"  << endl;
}