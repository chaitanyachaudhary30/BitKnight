#include <iostream>

#include "fen.h"
#include "move_gen.h"

int main() {
    using namespace BitKnight;

    Position pos;

    const std::string TEST_FEN =
        "4k3/8/8/3pP3/8/8/8/R3K2R w KQ d6 0 1";

    if (!loadFEN(pos, TEST_FEN)) {
        std::cout << "Failed to load test FEN.\n";
        return 1;
    }

    pos.print();

    MoveList moves;
    generatePseudoLegalMoves(pos, moves);

    int kingCastleCount = 0;
    int queenCastleCount = 0;
    int enPassantCount = 0;

    for (const Move& move : moves) {
        if (move.flag == MoveFlag::KingCastle) {
            kingCastleCount++;
        } else if (move.flag == MoveFlag::QueenCastle) {
            queenCastleCount++;
        } else if (move.flag == MoveFlag::EnPassant) {
            enPassantCount++;
        }
    }

    std::cout << "Generated pseudo-legal moves: "
              << moves.size() << "\n";

    std::cout << "Kingside castle moves: "
              << kingCastleCount << "\n";

    std::cout << "Queenside castle moves: "
              << queenCastleCount << "\n";

    std::cout << "En passant moves: "
              << enPassantCount << "\n";

    std::cout << "\nExpected:\n";
    std::cout << "Total moves: 28\n";
    std::cout << "Kingside castle moves: 1\n";
    std::cout << "Queenside castle moves: 1\n";
    std::cout << "En passant moves: 1\n";

    return 0;
}