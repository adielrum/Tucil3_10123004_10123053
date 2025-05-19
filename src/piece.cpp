#include "piece.h"
using namespace std;

// Konstruktor
Piece::Piece() : name(' '), pos_x(0), pos_y(0), len(0), ori(0) {}
Piece::Piece(int _pos_x, int _pos_y, int _len, int _ori, char _name)
    : pos_x(_pos_x), pos_y(_pos_y), len(_len), ori(_ori), name(_name) {}

// Display piece
void Piece::displayPiece() {
    cout << "huruf: " << name
         << " baris: " << pos_x
         << " kolom: " << pos_y
         << " panjang: " << len
         << " orientasi: " << ((ori == 0) ? "horizontal" : "vertical")
         << endl;
}