#pragma once

#include "position.h"
#include "move.h"

namespace BitKnight {

    // After searching, this returns the best move as well the evalution score after that move.
    struct SearchResult {
    Move bestMove;  
    int score = 0;
    bool hasMove = false;  // If checkmate/stalemate, then no move to return.
};

/*
    Finds the best move for the side to move.

    depth:
        How many moves ahead to search.

    Example:
        depth = 1 means BitKnight looks only at its own move.
        depth = 2 means BitKnight looks at its move and opponent's reply.
        depth = 3 means BitKnight looks one move deeper again.

    Returns:
        Best Move found.
        If no legal move exists, returns default Move().
*/
SearchResult searchBestMove(const Position& pos, int depth);

}