/* A chess position needs to remember:
1) Where every piece is
2) Whose turn it is 
3) Castling rights
4) Identify any En passant square
5) Check for the 50 move rule (Halfmove clock)
6) Count the move number (Fullmove number)

Doesn't yet include position remembering for 3-fold repetetion.
Logic for detecting checkmate, check, stalemate etc will go in later files dedicated specifically for attack detection, legal move generation etc. This file only contains the basic things.
position.h has all the functions and variables, positions.cpp will tell how they interact.
This file mainly contains majorly the fields of an FEN representation.
Put some attribute in this file only if it is necessary to describe the current chess state, don't move it if it requires move .
50 move rule also requires history, but only the number of moves since the last capture/pawn push, not the exact positions like 3 fold repetetion.
Checkmate/stalemate are not independent stored data, they are answers we compute. bool is_CheckMate() would beimpossible to right now in position.cpp, since we would need attack detection, legal move detection etc.
*/

#pragma once

#include <array>

#include "types.h"

namespace BitKnight {

struct Position { // struct is public by default, unlike class. So use struct.
    std::array<Bitboard, 12> pieces; // Every piece has a bitboard, corresponding to the bit = 1 in the uint (bitboard).

    Bitboard whitePieces; // Every board has white pieces and black pieces.
    Bitboard blackPieces;
    Bitboard occupied; // Every position has a bitboard representation, with the bits = 1 denoting occupied.

    Color sideToMove;

    int castlingRights;
    Square enPassantSquare; // if no square available for enPassant, Square :: None.

    int halfmoveClock;
    int fullmoveNumber;

    Position(); // Constructor {We are just writing here that constructor is also part of the general schema}

    void clear();
    void updateOccupancies();

    void setPiece(Piece piece, Square square);
    void removePiece(Piece piece, Square square);

    bool isOccupied(Square square) const; // const at the end means this function will not modify the board.
    bool isOccupiedByColor(Square square, Color color) const;

    Piece pieceAt(Square square) const;

    void print() const;
};

}