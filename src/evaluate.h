#pragma once

#include "position.h"

namespace BitKnight {

int evaluate(const Position& pos);
int pieceValue(Piece piece);
int pieceTypeValue(PieceType pieceType);

// Evaluates the position, wrt to material value only for now.
// positive means white is better, negative means black.

}