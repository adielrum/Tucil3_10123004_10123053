#include <bits/stdc++.h>
#include "piece.h"
#include "move.h"
#include "papan.h"
#include "state.h"
#include "algorithm.h"

using namespace std;

int N, M, num_piece;

int main()
{
    cout << "Input: " << flush;
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
    vector<pair<Piece, Move>> solution = AStar(current_state);

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