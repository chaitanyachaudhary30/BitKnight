#include "evaluate.h"

namespace BitKnight {

int pieceValue(Piece piece) {
    switch (piece) {
        case Piece::WhitePawn:
        case Piece::BlackPawn:
            return 100;

        case Piece::WhiteKnight:
        case Piece::BlackKnight:
            return 320;

        case Piece::WhiteBishop:   // Value bishops a bit more than knights in general.
        case Piece::BlackBishop:
            return 330;            

        case Piece::WhiteRook:
        case Piece::BlackRook:
            return 500;

        case Piece::WhiteQueen:
        case Piece::BlackQueen:
            return 900;

        // case Piece::WhiteKing:
        // case Piece::BlackKing:
        //     return 0;

        default:
            return 0;
    }
}

int pieceTypeValue(PieceType pieceType) {
    switch (pieceType) {
        case PieceType::Pawn:
            return 100;

        case PieceType::Knight:
            return 320;

        case PieceType::Bishop:   
            return 330;            

        case PieceType::Rook:
            return 500;

        case PieceType::Queen:
            return 900;

        default:
            return 0;
    }
}

// Mirrors square vertically for black pieces.
// Since Square::A1 = 0 and Square::A8 = 56, xor 56 flips ranks.
static int mirrorSquareIndex(int sqIndex) {
    return sqIndex ^ 56;
}

// Basic piece-square tables from White's perspective.
// Index 0 = a1, index 63 = h8.

// Pawns like advancing and controlling central files.

static const int PAWN_SQUARE_BONUS[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
      5,  10,  10, -20, -20,  10,  10,   5,
      5,  -5, -10,   0,   0, -10,  -5,   5,
      0,   0,   0,  20,  20,   0,   0,   0,
      5,   5,  10,  25,  25,  10,   5,   5,
     10,  10,  20,  30,  30,  20,  10,  10,
     50,  50,  50,  50,  50,  50,  50,  50,
      0,   0,   0,   0,   0,   0,   0,   0
};

// Knights are best near the center and bad on edges/corners.
static const int KNIGHT_SQUARE_BONUS[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20,   0,   5,   5,   0, -20, -40,
    -30,   5,  10,  15,  15,  10,   5, -30,
    -30,   0,  15,  20,  20,  15,   0, -30,
    -30,   5,  15,  20,  20,  15,   5, -30,
    -30,   0,  10,  15,  15,  10,   0, -30,
    -40, -20,   0,   0,   0,   0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50
};

// Bishops like open diagonals and central squares.
static const int BISHOP_SQUARE_BONUS[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10,   5,   0,   0,   0,   0,   5, -10,
    -10,  10,  10,  10,  10,  10,  10, -10,
    -10,   0,  10,  10,  10,  10,   0, -10,
    -10,   5,   5,  10,  10,   5,   5, -10,
    -10,   0,   5,  10,  10,   5,   0, -10,
    -10,   0,   0,   0,   0,   0,   0, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
};

// Rooks like open/semi-open ranks/files, but this simple table mainly rewards activity.
static const int ROOK_SQUARE_BONUS[64] = {
      0,   0,   5,  10,  10,   5,   0,   0,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
      5,  10,  10,  10,  10,  10,  10,   5,
      0,   0,   0,   5,   5,   0,   0,   0
};

// Queens get small centralization bonuses.
static const int QUEEN_SQUARE_BONUS[64] = {
    -20, -10, -10,  -5,  -5, -10, -10, -20,
    -10,   0,   5,   0,   0,   0,   0, -10,
    -10,   5,   5,   5,   5,   5,   0, -10,
      0,   0,   5,   5,   5,   5,   0,  -5,
     -5,   0,   5,   5,   5,   5,   0,  -5,
    -10,   0,   5,   5,   5,   5,   0, -10,
    -10,   0,   0,   0,   0,   0,   0, -10,
    -20, -10, -10,  -5,  -5, -10, -10, -20
};

// Basic king table for middlegame: safer near back rank/corners.
static const int KING_SQUARE_BONUS[64] = {
     20,  30,  10,   0,   0,  10,  30,  20,
     20,  20,   0,   0,   0,   0,  20,  20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30
};

static int pieceSquareBonus(Piece piece, int sqIndex) {
    Color color = pieceColor(piece);

    // PSTs are written from White's perspective.
    // For black pieces, mirror the square so black gets equivalent bonuses.
    int tableIndex = (color == Color::White) ? sqIndex : mirrorSquareIndex(sqIndex);

    switch (piece) {
        case Piece::WhitePawn:
        case Piece::BlackPawn:
            return PAWN_SQUARE_BONUS[tableIndex];

        case Piece::WhiteKnight:
        case Piece::BlackKnight:
            return KNIGHT_SQUARE_BONUS[tableIndex];

        case Piece::WhiteBishop:
        case Piece::BlackBishop:
            return BISHOP_SQUARE_BONUS[tableIndex];

        case Piece::WhiteRook:
        case Piece::BlackRook:
            return ROOK_SQUARE_BONUS[tableIndex];

        case Piece::WhiteQueen:
        case Piece::BlackQueen:
            return QUEEN_SQUARE_BONUS[tableIndex];

        case Piece::WhiteKing:
        case Piece::BlackKing:
            return KING_SQUARE_BONUS[tableIndex];

        default:
            return 0;
    }
}


/*
This evaluate function was till version 0.2.3

int evaluate(const Position& pos) {
    int score = 0;

    for (int sqIndex = 0; sqIndex < 64; sqIndex++) {
        Square sq = static_cast<Square>(sqIndex);

        if (!pos.isOccupied(sq)) {
            continue;
        }

        Piece piece = pos.pieceAt(sq);
        int value = pieceValue(piece);

        if (pieceColor(piece) == Color::White) {
            score += value;
        } else if (pieceColor(piece) == Color::Black) {  // Subtract black piece's value.
            score -= value;
        }
    }

    // Negamax expects evaluation from the side-to-move's perspective.
    if (pos.sideToMove == Color::White) {
        return score;
    } 
    else {
        return -score;
    }
}

}
*/


// NOW THE BELOW EVALUATE WILL WORK FOR 0.2.4.
int evaluate(const Position& pos) {
    int score = 0;

    for (int sqIndex = 0; sqIndex < 64; sqIndex++) {
        Square sq = static_cast<Square>(sqIndex);

        if (!pos.isOccupied(sq)) {
            continue;
        }

        Piece piece = pos.pieceAt(sq);

        int value = pieceValue(piece);
        value += pieceSquareBonus(piece, sqIndex);

        if (pieceColor(piece) == Color::White) {
            score += value;
        } 
        else if (pieceColor(piece) == Color::Black) {
            score -= value;
        }
    }

    // Return score from side-to-move's perspective for negamax.
    if (pos.sideToMove == Color::White) {
        return score;
    }

    return -score;
}
}