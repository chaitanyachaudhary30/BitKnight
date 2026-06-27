#include "evaluate.h"

namespace BitKnight {

static int pieceValue(Piece piece) {
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