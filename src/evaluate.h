#pragma once

#include "position.h"

namespace BitKnight {

int evaluate(const Position& pos);

// Evaluates the position, wrt to material value only for now.
// positive means white is better, negative means black.

}