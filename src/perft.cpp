#include "perft.h"

#include <iostream>
#include <string>

#include "legal_moves.h"
#include "make_move.h"

namespace BitKnight {

/*
    Converts an internal Square enum to UCI square notation.

    Examples:
        Square::E2 -> "e2"
        Square::G1 -> "g1"

    This is only used for readable divide output.
*/
static std::string squareToUci(Square square) {
    int index = static_cast<int>(square);

    if (index < 0 || index >= 64) {
        return "none";
    }

    char file = static_cast<char>('a' + (index % 8));
    char rank = static_cast<char>('1' + (index / 8));

    std::string result;
    result += file;
    result += rank;

    return result;
}

/*
    Converts promotion type to UCI promotion character.

    UCI promotion notation:
        e7e8q = promote to queen
*/
static char promotionChar(PieceType promotion) {
    if (promotion == PieceType::Queen) return 'q';
    if (promotion == PieceType::Rook) return 'r';
    if (promotion == PieceType::Bishop) return 'b';
    if (promotion == PieceType::Knight) return 'n';

    return '\0';
}

/*
    Converts internal Move object to UCI move notation.

    Examples:
        E2 -> E4          becomes "e2e4"
        G1 -> F3          becomes "g1f3"
        E7 -> E8 = Queen  becomes "e7e8q"

    This is useful for divide output.
*/
static std::string moveToUci(const Move& move) {
    std::string result;

    result += squareToUci(move.from);
    result += squareToUci(move.to);

    if (isPromotion(move)) {
        char promo = promotionChar(move.promotion);

        if (promo != '\0') {
            result += promo;
        }
    }

    return result;
}

/*
    Recursive perft function.

    Core idea:
        - Generate all legal moves.
        - If depth == 1, return number of legal moves.
        - Otherwise, make each move on a copied position and recurse.

    We use Position copy = pos because this engine currently does not have
    make/unmake move yet. Copying is slower but much safer for now.
*/
std::uint64_t perft(const Position& pos, int depth) {
    // One leaf node reached.
    if (depth == 0) {
        return 1;
    }

    MoveList legalMoves;
    generateLegalMoves(pos, legalMoves);

    // At depth 1, the node count is simply the number of legal moves.
    if (depth == 1) {
        return static_cast<std::uint64_t>(legalMoves.size());
    }

    std::uint64_t nodes = 0;

    for (const Move& move : legalMoves) {
        Position copy = pos;

        // This should normally succeed because moves are already legal.
        // Still keep the check as a defensive safeguard.
        if (!makeMove(copy, move)) { // By calling makeMove, the move is already made on the board. So depth has already decreased in essence.
            continue;                // Which is why we pass copy in the next line, not call makeMove() again.
        }

        nodes += perft(copy, depth - 1);
    }

    return nodes;
}


/*
    Below function prints perft count move-by-move from the current root position.
*/
void perftDivide(const Position& pos, int depth) {
    if (depth <= 0) {
        std::cout << "Total nodes: 1\n";
        return;
    }

    MoveList legalMoves;
    generateLegalMoves(pos, legalMoves);

    std::uint64_t totalNodes = 0;

    for (const Move& move : legalMoves) {
        Position copy = pos;

        if (!makeMove(copy, move)) {
            continue;
        }

        std::uint64_t moveNodes = perft(copy, depth - 1);
        totalNodes += moveNodes;

        std::cout << moveToUci(move) << ": " << moveNodes << "\n";
    }

    std::cout << "Total nodes: " << totalNodes << "\n";
}

}