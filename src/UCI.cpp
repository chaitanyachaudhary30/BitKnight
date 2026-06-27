#include "uci.h"

#include <iostream>
#include <sstream>
#include <string>

#include "fen.h"
#include "legal_moves.h"
#include "make_move.h"
#include "search.h"

namespace BitKnight {

static int given_searchDepth = 3;

// Reads depth from commands like:
// go depth 4
// If no depth is given, uses g_searchDepth.
static int parseDepthFromGoCommand(const std::string& line) {
    std::istringstream iss(line);

    std::string token;
    int depth = given_searchDepth;

    while (iss >> token) {
        if (token == "depth") {
            if (iss >> depth) {
                if (depth <= 0) {
                    depth = 1;
                }
            }

            break;
        }
    }

    return depth;
}

// Handles commands like:
// setoption name Search Depth value 4
static void handleSetOptionCommand(const std::string& line) {
    if (line.find("Search Depth") == std::string::npos) {
        return;
    }

    std::size_t valuePos = line.find("value");

    if (valuePos == std::string::npos) {
        return;
    }

    std::istringstream iss(line.substr(valuePos + 5));

    int newDepth;

    if (iss >> newDepth) {
        if (newDepth <= 0) {
            newDepth = 1;
        }

        given_searchDepth = newDepth;
    }
}

// Converts internal Square enum to UCI square text.
// Example: Square::E2 -> "e2"
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
// UCI promotion letters are lowercase: q, r, b, n.
static char promotionChar(PieceType promotion) {
    if (promotion == PieceType::Queen) return 'q';
    if (promotion == PieceType::Rook) return 'r';
    if (promotion == PieceType::Bishop) return 'b';
    if (promotion == PieceType::Knight) return 'n';

    return '\0';
}

// Converts internal Move to UCI format.
// Examples: e2e4, e7e8q
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

// Applies a GUI move by matching it against generated legal moves.
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

// Handles:
// position startpos
// position startpos moves e2e4 e7e5
// position fen <fen>
// position fen <fen> moves ...
static void handlePositionCommand(Position& pos, const std::string& line) {
    std::istringstream ss(line);

    std::string token;
    ss >> token; // "position"
    ss >> token; // "startpos" or "fen"

    if (token == "startpos") {
        loadFEN(pos, STARTING_FEN);

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

        // UCI FEN has 6 fields.
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

// Handles UCI "go" command.
// v0.2.0: fixed-depth search using searchBestMove().
static void handleGoCommand(Position& pos, const std::string& line) {
    int depth = parseDepthFromGoCommand(line);

    SearchResult result = searchBestMove(pos, depth);

    if (!result.hasMove) {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    std::string bestMoveString = moveToUci(result.bestMove);

    std::cout << "info depth " << depth
              << " score cp " << result.score
              << std::endl;

    std::cout << "bestmove " << bestMoveString << std::endl;

    // Useful for manual terminal testing.
    // Cute Chess usually sends the full updated position again.
    makeMove(pos, result.bestMove);
}

// Main UCI loop.
void runUciLoop() {
    Position pos;
    loadFEN(pos, STARTING_FEN);

    std::string line;

    while (std::getline(std::cin, line)) {
        if (line == "uci") {
            std::cout << "id name BitKnight" << std::endl;
            std::cout << "id author Chaitanya Chaudhary" << std::endl;

            // Cute Chess can show this option before the game starts.
            std::cout << "option name Search Depth type spin default 3 min 1 max 8" << std::endl;

            std::cout << "uciok" << std::endl;
        }
        else if (line == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (line == "ucinewgame") {
            loadFEN(pos, STARTING_FEN);
        }
        else if (line.rfind("setoption", 0) == 0) {
            handleSetOptionCommand(line);
        }
        else if (line.rfind("position", 0) == 0) {
            handlePositionCommand(pos, line);
        }
        else if (line.rfind("go", 0) == 0) {
            handleGoCommand(pos, line);
        }
        else if (line == "quit") {
            break;
        }
    }
}

}