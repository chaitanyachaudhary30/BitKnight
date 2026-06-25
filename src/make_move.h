#pragma once

#include "position.h"
#include "move.h"


// This deals with all the types of moves.
/*
    It also updates:
    - side to move
    - en passant square
    - castling rights
    - halfmove clock
    - fullmove number
    - occupancy bitboards

    Returns true if move was applied, false if couldn't be applied.
    This function does NOT yet check whether the move is legal.
*/

namespace BitKnight {

    bool makeMove (Position& pos, const Move& move);

}

