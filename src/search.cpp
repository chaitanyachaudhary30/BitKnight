#include "search.h"

#include "evaluate.h"
#include "legal_moves.h"
#include "make_move.h"
#include "attack.h"

namespace BitKnight {

static constexpr int INF = 1000000000;
static constexpr int MATE_SCORE = 100000;

/*
 Searches the position from the current side's perspective.
Return a positive score, if the move results in a good evaluation for the SIDE TO MOVE.
plyFromRoot = number of half-moves searched from the original root position.
It is used to prefer faster mates and delay losing mates.
*/

static int negamax(const Position& pos, int depth, int alpha, int beta, int plyFromRoot) {
    if (depth <= 0) {
        return evaluate(pos);
    }

    MoveList legalMoves;
    generateLegalMoves(pos, legalMoves);

    if (legalMoves.empty()) {
        // No legal moves + king in check = checkmate.
        // This is very bad for the side to move.
        if (isKingInCheck(pos, pos.sideToMove)) {
            return -MATE_SCORE + plyFromRoot;
        }

        // No legal moves + king not in check = stalemate.
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
        int score = -negamax(child, depth - 1, -beta, -alpha, plyFromRoot + 1);

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
    Move bestMove = legalMoves[0];

    for (const Move& move : legalMoves) {
        Position child = pos;

        if (!makeMove(child, move)) {
            continue;
        }

        int score = -negamax(child, depth - 1, -INF, INF, 1);

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