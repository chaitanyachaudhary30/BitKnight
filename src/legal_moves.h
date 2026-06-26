#pragma once

#include "move_gen.h"

namespace BitKnight {

/*
    Generates fully legal moves for the current position.

    Legal moves are produced by:
    1. generating all pseudo-legal moves from the position pos.
    2. making each move on a copied position Position copy = pos
    3. If the move is legal, push_back it in MoveList legalMoves.

*/

void generateLegalMoves(const Position& pos, MoveList& legalMoves);

}