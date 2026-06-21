#include <iostream>
#include "position.h"
#include "fen.h"

int main() {
    using namespace BitKnight;

    Position pos;

    pos.setPiece(Piece::WhiteKing, Square::E1);
    pos.setPiece(Piece::WhiteRook, Square::H1);
    pos.setPiece(Piece::BlackKing, Square::E8);
    pos.setPiece(Piece::BlackQueen, Square::D8);
    pos.setPiece(Piece::WhitePawn, Square::E4);

    pos.print();

    std::cout << std::boolalpha; // boolaplha prints all 0's and 1's of boolean outputs as "true" or "false".

    std::cout << "Is E4 occupied? " << pos.isOccupied(Square::E4) << "\n";

    std::cout << "Is E4 occupied by White? " << pos.isOccupiedByColor(Square::E4, Color::White) << "\n\n\n";




    if (!(loadFEN (pos, STARTING_FEN))) {
        std :: cout << "Failed to load FEN.\n";
        return 1;
    }
    std :: cout << "Starting postion loaded successfully:\n";
    pos.print(); 

    std::cout << "Is E4 occupied by Black? " << pos.isOccupiedByColor(Square::E4, Color::Black) << "\n";

    std::cout << "Is A1 occupied? " << pos.isOccupied(Square::A1) << "\n";


    return 0;
}