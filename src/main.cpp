#include <bits/stdc++.h>
#include "piece.h"
#include "move.h"
#include "papan.h"
#include "state.h"
#include "algorithm.h"

using namespace std;

int main()
{
    while (true) {
        string file_path;

        ifstream input_file(file_path);
        do {
            cout << "File path: ";
            cin >> file_path;
            input_file.close(); // Close the previous attempt
            input_file.open(file_path); // Try to open with the new path
        } while (!input_file.is_open());

        int N, M, num_piece;
        input_file >> N >> M >> num_piece;
        bool ketemu_k = false;
        vector<string> temp_board;
        string temp_row;
        input_file.ignore(numeric_limits<streamsize>::max(), '\n');
        while(getline(input_file, temp_row)){
            temp_board.push_back(temp_row);
            // cout << temp_row << endl;
        }

        // kalo belom ada 'K', berarti ada di baris bawah
        // if (ketemu_k == false) {
        //     string temp_row;
        //     input_file.ignore(numeric_limits<streamsize>::max(), '\n');
        //     getline(input_file, temp_row);
        //     temp_board.push_back(temp_row);
        // }

        Papan board = Papan(temp_board, N, M);
        vector<Piece> pieces = board.extractPieces();
        vector<pair<Piece, Move>> moves;
        State current_state(board, moves, pieces, 0);

        // cout << board.exit_x << " " << board.exit_y << endl;

        board.printGrid();

        vector<pair<Piece, Move>> solution = AStar(current_state);

        if (!solution.empty()) {
            for (size_t i = 0; i < solution.size(); i++) {
                cout << "Move " << (i+1) << ": ";
                cout << "Piece " << solution[i].first.name << " moves ";
                cout << solution[i].second.arah << " by " << solution[i].second.dist << endl;
                current_state.applyMove(solution[i]).papan.printGrid();
                cout << endl;
            }
        } else {
            cout << "\nNo solution found." << endl;
        }
        cout << "\n"  << endl;
    }

}