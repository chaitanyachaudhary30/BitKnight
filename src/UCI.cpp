#include "uci.h"

#include <iostream>
#include <sstream>
#include <string>
#include <random>

#include "fen.h"
#include "legal_moves.h"
#include "make_move.h"

namespace BitKnight {

// Converts internal Square enum to UCI square text.
// Eg: Square::E2 -> "e2"
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

// Converts promotion PieceType to UCI promotion character.
// UCI uses lowercase promotion letters: q, r, b, n.
static char promotionChar(PieceType promotion) {
    if (promotion == PieceType::Queen) return 'q';
    if (promotion == PieceType::Rook) return 'r';
    if (promotion == PieceType::Bishop) return 'b';
    if (promotion == PieceType::Knight) return 'n';

    return '\0';
}

// Converts BitKnight's internal Move object to UCI move format.
// Examples:
// e2 -> e4 becomes "e2e4"
// e7 -> e8 = Queen becomes "e7e8q"
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

// Applies a move received from the GUI.
// Instead of manually parsing every move detail, we generate legal moves
// and find the legal move whose UCI string matches the GUI move.
static bool makeUciMove(Position& pos, const std::string& uciMove) {
    MoveList legalMoves;
    generateLegalMoves(pos, legalMoves);

    for (const Move& move : legalMoves) {
        if (moveToUci(move) == uciMove) {
            return makeMove(pos, move);
        }
    }

    return false;
}

// Handles UCI "position" command.
// Examples:
// position startpos
// position startpos moves e2e4 e7e5
// position fen 4k3/8/8/8/8/8/8/4K3 w - - 0 1
static void handlePositionCommand(Position& pos, const std::string& line) {
    std::istringstream ss(line);

    std::string token;
    ss >> token; // reads "position"

    ss >> token; // reads either "startpos" or "fen"

    if (token == "startpos") {
        loadFEN(pos, STARTING_FEN);

        // After "startpos", GUI may optionally send "moves ..."
        if (ss >> token) {
            if (token == "moves") {
                std::string moveString;

                while (ss >> moveString) {
                    makeUciMove(pos, moveString);
                }
            }
        }
    } 
    else if (token == "fen") {
        std::string fenPart;
        std::string fen;

        // UCI FEN has exactly 6 fields:
        // board side castling enPassant halfmove fullmove
        for (int i = 0; i < 6; i++) {
            if (!(ss >> fenPart)) {
                return;
            }

            if (i > 0) {
                fen += " ";
            }

            fen += fenPart;
        }

        loadFEN(pos, fen);

        // After FEN, GUI may optionally send "moves ..."
        if (ss >> token) {
            if (token == "moves") {
                std::string moveString;

                while (ss >> moveString) {
                    makeUciMove(pos, moveString);
                }
            }
        }
    }
}

// Randomly chooses one move from the legal move list.
// This is temporary until we add evaluation/search.
static Move chooseRandomMove(const MoveList& moves) {
    static std::mt19937 rng(std::random_device{}());

    std::uniform_int_distribution<int> dist(
        0,
        static_cast<int>(moves.size()) - 1
    );

    return moves[dist(rng)];
}

// Handles UCI "go" command.
// For now, BitKnight simply chooses a random legal move.
static void handleGoCommand(Position& pos) {
    MoveList legalMoves;
    generateLegalMoves(pos, legalMoves);

    if (legalMoves.empty()) {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    Move chosenMove = chooseRandomMove(legalMoves);
    std::string bestMoveString = moveToUci(chosenMove);

    // Update internal position after choosing the move.
    makeMove(pos, chosenMove);

    // This is the main output the GUI expects.
    std::cout << "bestmove " << bestMoveString << std::endl;
}

// Main UCI loop.
// Continuously reads commands from GUI and prints UCI responses.
void runUciLoop() {
    Position pos;
    loadFEN(pos, STARTING_FEN);

    std::string line;

    while (std::getline(std::cin, line)) {
        if (line == "uci") {
            std::cout << "id name BitKnight" << std::endl;
            std::cout << "id author Chaitanya Chaudhary" << std::endl;
            std::cout << "uciok" << std::endl;
        } 
        else if (line == "isready") {
            std::cout << "readyok" << std::endl;
        } 
        else if (line == "ucinewgame") {
            loadFEN(pos, STARTING_FEN);
        } 
        else if (line.rfind("position", 0) == 0) {
            handlePositionCommand(pos, line);
        } 
        else if (line.rfind("go", 0) == 0) {
            handleGoCommand(pos);
        } 
        else if (line == "quit") {
            break;
        }
    }
}

}