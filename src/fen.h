/*
This header file will tell BitKnight that there is a function called loadFEN. 
It takes an FEN string, returns false if invalid, else it fills the board/Position according to the FEN notation and returns true.
*/

#pragma once

#include <string>
#include "position.h" // don't include position.cpp, we don't include .cpp files in headers.

namespace BitKnight {

inline constexpr const char* STARTING_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
// inline because we are writing inside a header file, constexpr because we know it at compile time, and const because we are not going to modify the string (READ Only).
// constexpr -> the pointer STARTING_FEN is a compile-time constant.
// const char* -> the characters it points to are read-only.

bool loadFEN(Position& pos, const std::string& fen);
// C++ will convert const char* of STARTING_FEN to string. 
}