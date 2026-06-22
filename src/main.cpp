#include <iostream>

#include "fen.h"
#include "move.h"

int main() {
    using namespace BitKnight;

    Position pos;

    if (!loadFEN(pos, STARTING_FEN)) {
        std::cout << "Failed to load FEN.\n";
        return 1;
    }

    std::cout << "Starting position loaded successfully:\n";
    pos.print();

    std::cout << std::boolalpha;

    // A normal pawn move from e2 to e4.
    Move quietMove(
        Square::E2,
        Square::E4,
        Piece::WhitePawn
    );

    // A knight capture example: white knight from f3 captures black pawn on e5.
    Move captureMove(
        Square::F3, //  source
        Square::E5, // destination
        Piece::WhiteKnight, // piece moving
        Piece::BlackPawn, // capture
        PieceType::None,  // promotion piece
        MoveFlag::Capture // what type of move
    );

    // A promotion example: white pawn promotes on e8 to a queen.
    Move promotionMove(
        Square::E7,
        Square::E8,
        Piece::WhitePawn,
        Piece::None,
        PieceType::Queen,
        MoveFlag::Promotion
    );

    std::cout << "quietMove is capture? "
              << isCapture(quietMove) << "\n";

    std::cout << "captureMove is capture? "
              << isCapture(captureMove) << "\n";

    std::cout << "promotionMove is promotion? "
              << isPromotion(promotionMove) << "\n";

    return 0;
}