#include "search.h"

#include "evaluate.h"
#include "move_gen.h"
#include "legal_moves.h"
#include "make_move.h"
#include "attack.h"

namespace BitKnight {

static constexpr int INF = 1000000000;

// Searches the position from the current side's perspective.
// Return a positive score, if the move results in a good evaluation for the SIDE TO MOVE.
static int negamax(const Position& pos, int depth, int alpha, int beta) {
    if (depth <= 0) {
        return evaluate(pos);
    }

    MoveList legalMoves;
    generateLegalMoves(pos, legalMoves);

    if (legalMoves.empty()) {
        // Temporary: later we will separate checkmate and stalemate.
        if (isK)
        return 0;
    }

    int bestScore = -INF;

    for (const Move& move : legalMoves) {
        Position child = pos;
        // Make this move on a copied position.
        // Since child is a copy, we do not need undoMove yet.

        if (!makeMove(child, move)) {
            continue;
        }


        // After our move, opponent gets the turn.
        // So we negate the opponent's score to get our score.
        int score = -negamax(child, depth - 1, -beta, -alpha);

        if (score > bestScore) {
            bestScore = score;
        }

        if (score > alpha) {
            alpha = score;   // alpha stores the best score found so far in this position.
        }

        // Alpha-beta pruning.
        if (alpha >= beta) {  // If alpha reaches beta, this branch cannot improve the final answer.
            break;
        }
    }

    return bestScore;
}

SearchResult searchBestMove(const Position& pos, int depth) {
        SearchResult result;  // Declaration of struct.
        MoveList legalMoves;
        generateLegalMoves(pos, legalMoves);

    if (legalMoves.empty()) {
        result.hasMove = false;
        result.score = 0;
        return result;
    }

    if (depth <= 0) {
        depth = 1;   // This is just for safety, depth will always be >= 1.
    }

    int bestScore = -INF;
    Move bestMove;

    for (const Move& move : legalMoves) {  
        Position child = pos;

        if (!makeMove(child, move)) {
            continue;
        }

        int score = -negamax(child, depth - 1, -INF, INF);

        if (score > bestScore) {  // for 1st valid move, bestScore would be -INF, so this evaluates true.
            bestScore = score;
            bestMove = move;
            result.hasMove = true;
        }
    }

    result.bestMove = bestMove;
    result.score = bestScore;

    return result;
}

}