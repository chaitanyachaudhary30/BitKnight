#pragma once

#include <vector>

#include "position.h"
#include "move.h"

namespace BitKnight {

    // MoveList is a list of Move objects.
    // This is similar to using bitBoard for uint.

using MoveList = std::vector<Move>;

    // Pseudo-legal means:
    // 1) Pieces move according to chess movement rules
    // 2) Pieces do not capture their own color
    // 2) Special moves like promotion/castling/en passant can be added later

    // They may still leave the moving side's king in check.
    // Legal move filtering will be added later using attack detection to see if king is not in check.

    void generatePseudoLegalMoves(const Position& pos, MoveList& moves);

}