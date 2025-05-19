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

        vector<string> temp_board;
        for(int i = 0; i < N; i++){
            string temp_row;
            input_file >> temp_row;
            temp_board.push_back(temp_row);
        }

        input_file.close();

        Papan board = Papan(temp_board, N, M);
        vector<Piece> pieces = board.extractPieces();
        vector<pair<Piece, Move>> moves;
        State current_state(board, moves, pieces, 0);

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