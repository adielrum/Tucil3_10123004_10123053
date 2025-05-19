#include "move.h"

Move::Move() : arah(" "), dist(0) {}

Move::Move(string _arah, int _dist)
    : arah(_arah), dist(_dist) {}

// Display move
void Move::displayMove() {
    cout << "arah: " << arah << " sebanyak " << dist << endl;
}
