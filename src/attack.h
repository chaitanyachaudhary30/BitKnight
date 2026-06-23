#pragma once

#include "position.h"

namespace BitKnight {


// This function does not care whose turn it is.
// It only checks whether a color attacks a square.
bool isSquareAttacked(const Position& pos, Square square, Color byColor);


// Finds the square of the king of the given color. If king not found, returns None.
Square findKing(const Position& pos, Color color);


// Returns true if the given color's king is currently in check by the opponent.
bool isKingInCheck(const Position& pos, Color color);

}