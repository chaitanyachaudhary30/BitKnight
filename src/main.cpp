#include <iostream>

#include "fen.h"
#include "move_gen.h"

int main() {
    using namespace BitKnight;

    Position pos;

    const std::string TEST_FEN =
        "4k3/8/8/8/3R4/8/8/4K3 w - - 0 1";

    if (!loadFEN(pos, TEST_FEN)) {
        std::cout << "Failed to load test FEN.\n";
        return 1;
    }

    pos.print();

    MoveList moves;
    generatePseudoLegalMoves(pos, moves);

    std::cout << "Generated pseudo-legal moves: "
              << moves.size() << "\n";

    std::cout << "Expected rook-only move count: 14\n";

    return 0;
}