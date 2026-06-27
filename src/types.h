#pragma once

#include <cstdint>

namespace BitKnight {

using Bitboard = std::uint64_t; // Bitboard is another name for uint64_t;

constexpr int BOARD_SIZE = 64;
constexpr int NO_SQUARE = 64;

enum class Color : int {
    White = 0,
    Black = 1,
    None = 2
};
// Using enum instead of map everywhere because enum takes constant time, whereas map takes log(n) lookup time.
// Also, map is only one way access, being key -> value. But with enum, if we have the piece, we can get the corresponding integer with toInt, and the vice-versa with static_cast<Square> (28).
enum class PieceType : int {
    Pawn = 0,
    Knight = 1,
    Bishop = 2,
    Rook = 3,
    Queen = 4,
    King = 5,
    None = 6 // None can be useful in situations like determining if there is no piece on the square.
};

enum class Piece : int {
    WhitePawn = 0,
    WhiteKnight = 1,
    WhiteBishop = 2,
    WhiteRook = 3,
    WhiteQueen = 4,
    WhiteKing = 5,

    BlackPawn = 6,
    BlackKnight = 7,
    BlackBishop = 8,
    BlackRook = 9,
    BlackQueen = 10,
    BlackKing = 11,

    None = 12
};

enum class Square : int {
    A1 = 0,  B1 = 1,  C1 = 2,  D1 = 3,  E1 = 4,  F1 = 5,  G1 = 6,  H1 = 7,
    A2 = 8,  B2 = 9,  C2 = 10, D2 = 11, E2 = 12, F2 = 13, G2 = 14, H2 = 15,
    A3 = 16, B3 = 17, C3 = 18, D3 = 19, E3 = 20, F3 = 21, G3 = 22, H3 = 23,
    A4 = 24, B4 = 25, C4 = 26, D4 = 27, E4 = 28, F4 = 29, G4 = 30, H4 = 31,
    A5 = 32, B5 = 33, C5 = 34, D5 = 35, E5 = 36, F5 = 37, G5 = 38, H5 = 39,
    A6 = 40, B6 = 41, C6 = 42, D6 = 43, E6 = 44, F6 = 45, G6 = 46, H6 = 47,
    A7 = 48, B7 = 49, C7 = 50, D7 = 51, E7 = 52, F7 = 53, G7 = 54, H7 = 55,
    A8 = 56, B8 = 57, C8 = 58, D8 = 59, E8 = 60, F8 = 61, G8 = 62, H8 = 63,

    None = 64
};

inline constexpr int toInt(Square square) {
    return static_cast<int>(square);
}

inline constexpr int toInt(Piece piece) {
    return static_cast<int>(piece);
}

inline constexpr int toInt(Color color) {
    return static_cast<int>(color); // returns 0 or 1 in integer form, for future bitwsise operations int would be required.
}

inline constexpr Color opposite(Color color) { // Whenever we will write the function for toggling the player to move, we will call this function.
    if (color == Color::White) return Color::Black;
    if (color == Color::Black) return Color::White; 
    return Color::None;
}

inline constexpr Bitboard squareBitboard(Square square) { // generates the bitboard (unsigned int) with the input square bit as being set to 1.
    return square == Square::None ? 0ULL : (1ULL << toInt(square));
}

inline constexpr Color pieceColor (Piece piece) {
    if (piece == Piece :: None) return Color :: None;
    int enum_value = static_cast <int> (piece);
    if (enum_value >= 0 && enum_value <= 5) return Color :: White;
    return Color :: Black;
}

inline constexpr PieceType Type_of_Piece (Piece piece) {
    int enum_val = (static_cast<int> (piece))%6;
    return static_cast<PieceType> (enum_val);
}

}



