// Define how BitKnight represents one chess move internally.
#pragma once

#include "types.h"

namespace BitKnight {

// MoveFlag tells us what type/category a move belongs to.

enum class MoveFlag : int {
    Quiet = 0,  // Can inflict check, but won't be capture, castle etc.
    Capture = 1,
    DoublePawnPush = 2,
    KingCastle = 3,
    QueenCastle = 4,
    EnPassant = 5,
    Promotion = 6,
    PromotionCapture = 7
};

// Move stores all information needed to describe a single chess move.
struct Move {
    Square from;          
    Square to;            

    Piece piece;          // The piece that is moving.
    Piece captured;       // The captured piece, Piece::None if no capture.

    PieceType promotion;  

    MoveFlag flag;        // Tells whether move is quiet, capture, castle, promotion, etc.

    // Default constructor.
    // Creates an empty move.
    // Useful when we need to declare a Move variable before assigning a real move.
    Move() {
        from = Square::None; 
        to = Square::None;   
        piece = Piece::None;
        captured = Piece::None;
        promotion = PieceType::None;
        flag = MoveFlag::Quiet; 
    }

    // Parameterized constructor.
    // Creates a real move with given values.
    Move(
        Square fromSquare,
        Square toSquare,
        Piece movingPiece,
        Piece capturedPiece = Piece::None,
        PieceType promotionType = PieceType::None, // Only answers whether its queen/rook, not white queen or white rook.
        MoveFlag moveFlag = MoveFlag::Quiet
    ) {
        from = fromSquare;
        to = toSquare;
        piece = movingPiece;
        captured = capturedPiece;
        promotion = promotionType;
        flag = moveFlag;
    }
};

// Returns true if this move captures an opponent piece.
inline bool isCapture(const Move& move){ // inline because this is a function inside .h file.
    MoveFlag moveflag = move.flag;
    if (moveflag == MoveFlag :: Capture || moveflag == MoveFlag :: EnPassant || moveflag == MoveFlag :: PromotionCapture) return true;
    return false;
}

// Returns true if this move is a pawn promotion.
inline bool isPromotion(const Move& move) {
    return move.flag == MoveFlag::Promotion ||
           move.flag == MoveFlag::PromotionCapture;
}

}