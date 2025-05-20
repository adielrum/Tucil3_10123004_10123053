#include <bits/stdc++.h>
#include "piece.h"
#include "move.h"
#include "papan.h"
#include "state.h"
#include "algorithm.h"

using namespace std;
using namespace chrono;

int main()
{
    while (true) {

        // Input file
        string file_path;
        ifstream input_file(file_path);
        do {
            cout << "File path: ";
            getline(cin, file_path); 
            input_file.close(); 
            input_file.open(file_path);
        } while (!input_file.is_open());
        cout << endl;

        // Read file 
        int N, M, num_piece;
        input_file >> N >> M >> num_piece;

        vector<string> temp_board;
        string temp_row;
        input_file.ignore(numeric_limits<streamsize>::max(), '\n'); // skip empty line 
        while(getline(input_file, temp_row)){
            temp_board.push_back(temp_row);
        }

        // Create papan 
        Papan board = Papan(temp_board, N, M);
        vector<Piece> pieces = board.extractPieces();
        vector<pair<Piece, Move>> moves;
        State current_state(board, moves, pieces, 0);

        // Papan awal
        cout << "Papan Awal" << endl;
        board.printGrid();
        cout << endl;

        // Solve papan 
        auto start = high_resolution_clock::now();
        vector<pair<Piece, Move>> solution = AStar(current_state);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        double time = duration.count() / 1000.0;
        cout << fixed << setprecision(3) << time << " ms. \n" << endl;

        // Print solution 
        if (!solution.empty()) {
            for (size_t i = 0; i < solution.size(); i++) {
                
                // Print move information
                cout << "Gerakan " << (i+1) << ": ";
                cout << "Pindah piece " << solution[i].first.name << " ke ";
                cout << solution[i].second.arah << " " << solution[i].second.dist << " langkah." << endl;

                // Print new state
                current_state = current_state.applyMove(solution[i]);
                current_state.papan.printGrid(solution[i].first.name);
                cout << endl;
            }
        } else {
            cout << "\nNo solution found." << endl;
        }
        cout << "\n"  << endl;
    }

}