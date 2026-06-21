#include "position.h"

#include <iostream>

namespace BitKnight {

static char pieceToChar(Piece piece) {
    switch (piece) {
        case Piece::WhitePawn:   return 'P';
        case Piece::WhiteKnight: return 'N';
        case Piece::WhiteBishop: return 'B';
        case Piece::WhiteRook:   return 'R';
        case Piece::WhiteQueen:  return 'Q';
        case Piece::WhiteKing:   return 'K';

        case Piece::BlackPawn:   return 'p';
        case Piece::BlackKnight: return 'n';
        case Piece::BlackBishop: return 'b';
        case Piece::BlackRook:   return 'r';
        case Piece::BlackQueen:  return 'q';
        case Piece::BlackKing:   return 'k';

        case Piece::None:        return '.';
    }

    return '.';
}

Position::Position() { // Constructor
    clear(); // By default, when we initialize the struct Position, this constructor will be called.
// For setting as the default starting position, we will create a different function.
}

void Position::clear() {
    pieces.fill(0ULL); // pieces was the 12 bitboard array we declared in pieces.h.
// Below are 3 summary bitboards, which will help us in fast lookups.
    whitePieces = 0ULL; 
    blackPieces = 0ULL;
    occupied = 0ULL;

    sideToMove = Color::White;

    castlingRights = 0;
    enPassantSquare = Square::None;

    halfmoveClock = 0;
    fullmoveNumber = 1;
}

void Position::updateOccupancies() { 
// Postion class has whitePieces, blackPieces etc as member functions, so we do not need to pass them inside updateOccupancies().
    whitePieces = 0ULL;
    blackPieces = 0ULL;
// After every move, we could update whitePieces, BlackPieces and occupied separately, in the makeMove() function, and that would be faster than always fully recomputing all of these.
// However, that would be more error prone, since that would depend on whether its a capture, whether the paw moved 2 places etc. Diff logic for updating these fields in every different type of move/capture.
    for (int i = toInt(Piece::WhitePawn); i <= toInt(Piece::WhiteKing); i++) {
        whitePieces |= pieces[i];
    }

    for (int i = toInt(Piece::BlackPawn); i <= toInt(Piece::BlackKing); i++) {
        blackPieces |= pieces[i];
    }

    occupied = whitePieces | blackPieces;
}

// The below funtion will help us in loading FEN positions etc, and will act as helper function in movePiece(). We don't also remove the prev position, because removePiece does that.
// To remove the old piece as well, we would need one more parameter, which is square_from.
// movePiece we will add in later versions,this is just the 1st version.
void Position::setPiece(Piece piece, Square square) {
    if (piece == Piece::None || square == Square::None) { // if piece or square not selected, just return.
        return;
    }

    pieces[toInt(piece)] |= squareBitboard(square);
    updateOccupancies();
}

void Position::removePiece(Piece piece, Square square) {
    if (piece == Piece::None || square == Square::None) {
        return;
    }

    pieces[toInt(piece)] &= ~squareBitboard(square);
    updateOccupancies();
}

bool Position::isOccupied(Square square) const { // const does not change the board.
    if (square == Square::None) {
        return false;
    }

    if ((occupied & squareBitboard(square)) == 0ULL) return false;
    return true;
}

bool Position::isOccupiedByColor(Square square, Color color) const {
    if (square == Square::None) {
        return false;
    }

    Bitboard squareMask = squareBitboard(square);

    if (color == Color::White) {
        if ((whitePieces & squareMask) != 0Ull) return true;
        return false;
    }

    if (color == Color::Black) {
        if ((blackPieces & squareMask) == 0ULL) return true;
        return false;
    }
    return true; // All cases already covered, this return true is just for maintaining the return type as bool.
}

Piece Position::pieceAt(Square square) const { // Checks which piece is at the square square.

    if (square == Square::None) {
        return Piece::None;
    }

    Bitboard squareMask = squareBitboard(square);

    for (int i = toInt(Piece::WhitePawn); i <= toInt(Piece::BlackKing); i++) {
        if ((pieces[i] & squareMask) != 0ULL) {
            return static_cast<Piece>(i);
        }
    }
// This function checks for upto 12 bitboards, so later we may need to optimize it.
    return Piece::None;
}

void Position::print() const {
    std::cout << "\n";

    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "  ";

        for (int file = 0; file < 8; file++) {
            int squareIndex = rank * 8 + file;
            Square square = static_cast<Square>(squareIndex);

            std::cout << pieceToChar(pieceAt(square)) << ' '; // In the initial setup, it will print all '.', as all pieceAt = none, and for none, pieceToChar returns '.'.
            
        }

        std::cout << "\n";
    }

    std::cout << "\n   a b c d e f g h\n\n";
}

}

// print() {
// for each rank from 8 to 1:
//     print rank number
//     for each file from a to h:
//         calculate square index
//         convert index to Square
//         find piece at that square
//         convert piece to character
//         print character
// print file letters a b c d e f g h
// }