#include <iostream>

#include "fen.h"
#include "move_gen.h"

int main() {
    using namespace BitKnight;

    Position pos;

    if (!loadFEN(pos, STARTING_FEN)) {
        std::cout << "Failed to load starting FEN.\n";
        return 1;
    }

    std::cout << "Starting position:\n";
    pos.print();

    MoveList moves;
    generatePseudoLegalMoves(pos, moves);

    std::cout << "Generated pseudo-legal moves: "
              << moves.size() << "\n";

    std::cout << "Expected pawn-only move count from starting position: 16\n";

    if (moves.size() == 16) {
        std::cout << "Pawn move generation test passed.\n";
    } else {
        std::cout << "Pawn move generation test failed.\n";
    }

    return 0;
}