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
        cout << "File path: ";
        cin >> file_path;

        ifstream input_file(file_path);
        while (!input_file.is_open()) {
            cout << "Could not open file! Try again: ";
            cin >> file_path;
            ifstream input_file(file_path);
        }

        int N, M, num_piece;
        input_file >> N >> M >> num_piece;

        bool ketemu_k = false;
        vector<string> temp_board;

        for(int i = 0; i < N; i++){
            string temp_row;
            input_file >> temp_row;
            temp_board.push_back(temp_row);

            // ngecek apakah ada 'K'
            for(int j = 0; j < temp_row.length(); j++) {
                if (temp_row[j] == 'K') ketemu_k = true;
            }
        }

        // kalo belom ada 'K', berarti ada di baris bawah
        if (ketemu_k == false) {
            // cout << "masuk sini" << flush;
            string temp_row;
            input_file.ignore(numeric_limits<streamsize>::max(), '\n'); 
            getline(input_file, temp_row);
            temp_board.push_back(temp_row);
            // cout << temp_row;
        }

        input_file.close();

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
            }
        } else {
            cout << "\nNo solution found." << endl;
        }
        cout << "\n"  << endl;
    }

}