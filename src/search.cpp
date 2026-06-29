#include "search.h"

#include <algorithm>

#include "evaluate.h"
#include "legal_moves.h"
#include "make_move.h"
#include "attack.h"

namespace BitKnight {

static constexpr int INF = 1000000000;
static constexpr int MATE_SCORE = 100000;


static int moveOrderScore(const Move& move) {
    int score = 0;

    // Promotions are searched early.
    // Queen promotions get more priority than underpromotions.
    if (isPromotion(move)) {
        score += 15000;
        score += pieceTypeValue(move.promotion);
    }

    // MVV-LVA: Most Valuable Victim - Least Valuable Attacker.
    if (move.captured != Piece::None) {
        int victimValue = pieceValue(move.captured);
        int attackerValue = pieceValue(move.piece);

        score += 10000;
        score += victimValue * 10 - attackerValue; // Capturing valuable pieces is more important. Next give priority to Using a cheaper piece for the capture
    }

    return score;
}
// Sorts legal moves so alpha-beta gets better pruning.
static void orderMoves(MoveList& legalMoves) {
    std::sort(
        legalMoves.begin(),
        legalMoves.end(),
        [](const Move& a, const Move& b) {
            return moveOrderScore(a) > moveOrderScore(b);
        }
    );
}

// Returns true for moves that should still be searched in quiescence.
// For now: captures and promotions.
static bool isCaptureOrPromotion(const Move& move) {
    return move.captured != Piece::None || isPromotion(move);
}


// Quiescence search avoids evaluating positions in the middle of captures.
// It searches only captures/promotions, unless the side to move is in check.


// alpha = best score found so far for the side to move {LOWER BOUND}
// beta  = score so good that opponent will avoid allowing this line {UPPER CUTOFF LIMIT}

static int quiescence(const Position& pos, int alpha, int beta, int plyFromRoot) {
    bool inCheck = isKingInCheck(pos, pos.sideToMove);

    // If not in check, we are allowed to evaluate the current position directly.
    // If in check, we cannot use staticEval because the side must respond to check.
    if (!inCheck) {
        int staticEval = evaluate(pos);

        if (staticEval >= beta) {
            return beta;
        }

        if (staticEval > alpha) {
            alpha = staticEval;
        }
    }

    MoveList legalMoves;
    generateLegalMoves(pos, legalMoves);

    if (legalMoves.empty()) {
        if (inCheck) {
            return -MATE_SCORE + plyFromRoot;
        }

        return 0;
    }

    orderMoves(legalMoves);

    for (const Move& move : legalMoves) {
        // If king is in check, search all legal evasions.
        // Otherwise, search only captures/promotions.
        if (!inCheck && !isCaptureOrPromotion(move)) {
            continue;
        }

        Position child = pos;

        if (!makeMove(child, move)) {
            continue;
        }

        int score = -quiescence(child, -beta, -alpha, plyFromRoot + 1);

        if (score >= beta) {
            return beta;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

// Searches the position from the current side's perspective.
// Return a positive score, if the move results in a good evaluation for the SIDE TO MOVE.
// plyFromRoot = number of half-moves searched from the original root position.
// It is used to prefer faster mates and delay losing mates.
static int negamax(const Position& pos, int depth, int alpha, int beta, int plyFromRoot) {
if (depth <= 0) {
    return quiescence(pos, alpha, beta, plyFromRoot);
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

    orderMoves(legalMoves);

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

    orderMoves(legalMoves);

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