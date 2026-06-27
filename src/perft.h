#pragma once

#include <cstdint> // for uint

#include "position.h"

namespace BitKnight {

/*
    perft(pos, depth)

    Counts the number of legal move sequences from a position.

    depth = 0:Return 1. This means we have reached one leaf node.

    depth = 1: Return number of legal moves in the current position.

    depth = 2: For each legal move, make it, then count opponent legal replies.
*/
std::uint64_t perft(const Position& pos, int depth);

/*
    perftDivide(pos, depth)

    Prints each root move and the number of leaf nodes under that move.

    Example at depth 2 from starting position:
        e2e4: 20
        d2d4: 20
        ...
        Total nodes: 400
    Useful for debugging when total perft is wrong and we need to locate the bad move branch.
*/
void perftDivide(const Position& pos, int depth);

}