#include <bits/stdc++.h>
#include "piece.h"
#include "move.h"
#include "papan.h"
#include "state.h"
#include "solver.h"

using namespace std;
using namespace chrono;

int main()
{
    cout << "\n=========================================================================================" << endl;
    cout << R"(
    _____           _       _    _                     _____       _                
    |  __ \         | |     | |  | |                   / ____|     | |               
    | |__) |   _ ___| |__   | |__| | ___  _   _ _ __  | (___   ___ | |_   _____ _ __ 
    |  _  / | | / __| '_ \  |  __  |/ _ \| | | | '__|  \___ \ / _ \| \ \ / / _ \ '__|
    | | \ \ |_| \__ \ | | | | |  | | (_) | |_| | |     ____) | (_) | |\ V /  __/ |   
    |_|  \_\__,_|___/_| |_| |_|  |_|\___/ \__,_|_|    |_____/ \___/|_| \_/ \___|_|   
    )" << endl;

    while (true) {

        cout << "=========================================================================================\n" << endl;

        // Input file
        string file_path;
        ifstream input_file(file_path);
        do {
            cout << "Masukan file (.txt): ";
            getline(cin, file_path); 
            input_file.close(); 
            input_file.open(file_path);
            if (!input_file.is_open()) {
                cout << "\nFile tidak bisa dibuka. Coba lagi. \n";
            }
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
        if (!board.is_valid) {
            cout << "Papan tidak valid. Coba lagi.\n";
            continue;
        }

        // Input algoritma
        string algoritma_type;
        cout << "Berikut beberapa algoritam pathfinding." << endl;
        cout << "  0. Uniform Cost Search (UCS)" << endl;
        cout << "  1. Greedy Best First Search" << endl;
        cout << "  2. A* Search" << endl;
        cout << "Pilih algoritma (0,1,2): ";
        while(getline(cin, algoritma_type)) {
            if (algoritma_type == "0" || algoritma_type == "1" || algoritma_type == "2") {
                break;
            }
            cout << "Pilihan algoritma tidak valid!" << endl;
            cout << "Pilih algoritma (0,1,2): ";
        }
        // State awal
        cout << "\nPapan Awal" << endl;
        board.printGrid();
        cout << endl;
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

        // Print solution 
        if (!solution.empty()) {
            for (size_t i = 0; i < solution.size(); i++) {
                
                pair<Piece, Move> frame = solution[i];

                // Print move information
                cout << "Gerakan " << (i+1) << ": ";
                cout << "Pindah piece " << frame.first.name << " ke ";
                cout << frame.second.arah << " " << frame.second.dist << " langkah." << endl;

                // Print new state
                current_state = current_state.applyMove(solution[i]);
                current_state.papan.printGrid(solution[i].first.name);
                cout << endl;
            }
        } 

    }

}