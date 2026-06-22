#include "move_gen.h"

namespace BitKnight {

    static const PieceType ReturnPieceType (Piece piece){
        if (piece == Piece :: None) return PieceType :: None;
        int enum_value = (static_cast <int> (piece))%6;
        return static_cast<PieceType> (enum_value);
    }


void generatePawnMoves(const Position& pos, MoveList& moves, Square source, Piece pawn) {
    int source_idx = static_cast<int>(source);

    int file = source_idx % 8;
    int rank = source_idx / 8;

    Color color = pos.sideToMove;

    int forward = (color == Color::White) ? 8 : -8;
    int double_forward = (color == Color::White) ? 16 : -16;

    int start_rank = (color == Color::White) ? 1 : 6;
    int promotion_rank = (color == Color::White) ? 6 : 1;

    int one_step_idx = source_idx + forward;

    // 1-square forward pawn push.
    if (one_step_idx >= 0 && one_step_idx < 64) {
        Square one_step_square = static_cast<Square>(one_step_idx);

        if (!pos.isOccupied(one_step_square)) {
            // If pawn reaches final rank, generate all 4 promotion options.
            if (rank == promotion_rank) {
                moves.push_back(Move(source, one_step_square, pawn, Piece::None, PieceType::Queen, MoveFlag::Promotion));
                moves.push_back(Move(source, one_step_square, pawn, Piece::None, PieceType::Rook, MoveFlag::Promotion));
                moves.push_back(Move(source, one_step_square, pawn, Piece::None, PieceType::Bishop, MoveFlag::Promotion));
                moves.push_back(Move(source, one_step_square, pawn, Piece::None, PieceType::Knight, MoveFlag::Promotion));
            } else {
                moves.push_back(Move(source, one_step_square, pawn));

                // 2-square pawn push from starting rank.
                int two_step_idx = source_idx + double_forward;

                if (rank == start_rank && two_step_idx >= 0 && two_step_idx < 64) {
                    Square two_step_square = static_cast<Square>(two_step_idx);

                    if (!pos.isOccupied(two_step_square)) {
                        moves.push_back(Move(source, two_step_square, pawn, Piece::None, PieceType::None,MoveFlag::DoublePawnPush));
                    }
                }
            }
        }
    }

    // Pawn capture directions.
    int capture_towards_A = (color == Color::White) ? 7 : -9; // Left capture for White, Right capture for black.
    int capture_towards_H = (color == Color::White) ? 9 : -7; // Right capture for White, Left for Black.

    // capture_towards_A: only allowed if pawn is not on A-file.
    if (file > 0) {
        int target_idx = source_idx + capture_towards_A;

        if (target_idx >= 0 && target_idx < 64) {
            Square target_square = static_cast<Square>(target_idx);

            if (pos.isOccupiedByColor(target_square, opposite(color))) {
                Piece captured_piece = pos.pieceAt(target_square);

                if (rank == promotion_rank) {
        // Call the custom Move constructor for every type of promoted piece, then add that move to our vector of moves.
                    moves.push_back(Move(source, target_square, pawn, captured_piece, PieceType::Queen, MoveFlag::PromotionCapture));
                    moves.push_back(Move(source, target_square, pawn, captured_piece, PieceType::Rook, MoveFlag::PromotionCapture));
                    moves.push_back(Move(source, target_square, pawn, captured_piece, PieceType::Bishop, MoveFlag::PromotionCapture));
                    moves.push_back(Move(source, target_square, pawn, captured_piece, PieceType::Knight, MoveFlag::PromotionCapture));
                } else {
                    moves.push_back(Move(
                        source,
                        target_square,
                        pawn,
                        captured_piece,
                        PieceType::None,
                        MoveFlag::Capture
                    ));
                }
            }
        }
    }

    // capture_towards_H: only allowed if pawn is not on H-file.
    if (file < 7) {
        int target_idx = source_idx + capture_towards_H;

        if (target_idx >= 0 && target_idx < 64) {
            Square target_square = static_cast<Square>(target_idx);

            if (pos.isOccupiedByColor(target_square, opposite(color))) {
                Piece captured_piece = pos.pieceAt(target_square);

                if (rank == promotion_rank) {
                    moves.push_back(Move(source, target_square, pawn, captured_piece, PieceType::Queen, MoveFlag::PromotionCapture));
                    moves.push_back(Move(source, target_square, pawn, captured_piece, PieceType::Rook, MoveFlag::PromotionCapture));
                    moves.push_back(Move(source, target_square, pawn, captured_piece, PieceType::Bishop, MoveFlag::PromotionCapture));
                    moves.push_back(Move(source, target_square, pawn, captured_piece, PieceType::Knight, MoveFlag::PromotionCapture));
                } else {
                    moves.push_back(Move(
                        source,
                        target_square,
                        pawn,
                        captured_piece,
                        PieceType::None,
                        MoveFlag::Capture
                    ));
                }
            }
        }
    }
}        

// KNIGHT
void generateKnightMoves(const Position& pos, MoveList& moves, Square source, Piece knight) {
    int source_idx = static_cast<int>(source);

    int file = source_idx % 8;
    int rank = source_idx / 8;

    // All 8 possible knight jumps as {file change, rank change}.
    int knight_moves[8][2] = {
        { 1,  2},
        {-1,  2},
        { 1, -2},
        {-1, -2},
        { 2,  1},
        {-2,  1},
        { 2, -1},
        {-2, -1}
    };

    for (int i = 0; i < 8; i++) {
        int target_file = file + knight_moves[i][0];
        int target_rank = rank + knight_moves[i][1];

        // If target square is outside the board, skip it.
        if (target_file < 0 || target_file > 7 || target_rank < 0 || target_rank > 7) {
            continue;
        }

        int target_idx = target_rank * 8 + target_file;
        Square target_square = static_cast<Square>(target_idx);

        Piece target_piece = pos.pieceAt(target_square);

        // Empty square: quiet knight move.
        if (target_piece == Piece::None) {
            moves.push_back(Move(
                source,
                target_square,
                knight,
                Piece::None,
                PieceType::None,
                MoveFlag::Quiet
            ));
        }

        // Opponent piece: capture move.
        else if (pieceColor(target_piece) == opposite(pos.sideToMove)) {
            moves.push_back(Move(
                source,
                target_square,
                knight,
                target_piece,
                PieceType::None,
                MoveFlag::Capture
            ));
        }

        // Otherwise it will be same color piece, so no move.
    }
}


// BISHOP
void generateBishopMoves(const Position& pos, MoveList& moves, Square source, Piece bishop) {
    int source_idx = static_cast<int>(source);

    int source_file = source_idx % 8;
    int source_rank = source_idx / 8;

    // Bishop moves in 4 diagonal directions:
    // top-right, top-left, bottom-right, bottom-left.
    int bishop_directions[4][2] = {
        { 1,  1},
        {-1,  1},
        { 1, -1},
        {-1, -1}
    };

    for (int dir = 0; dir < 4; dir++) {
        int file = source_file + bishop_directions[dir][0];
        int rank = source_rank + bishop_directions[dir][1];

        // Keep moving in this diagonal direction until blocked or outside board.
        while (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
            int target_idx = rank * 8 + file;
            Square target_square = static_cast<Square>(target_idx);

            Piece target_piece = pos.pieceAt(target_square);

            // Empty square: bishop can move here and continue sliding.
            if (target_piece == Piece::None) {
                moves.push_back(Move(
                    source,
                    target_square,
                    bishop,
                    Piece::None,
                    PieceType::None,
                    MoveFlag::Quiet
                ));
            }

            // Enemy piece: bishop can capture it, but cannot move beyond it.
            else if (pieceColor(target_piece) == opposite(pos.sideToMove)) {
                moves.push_back(Move(
                    source,
                    target_square,
                    bishop,
                    target_piece,
                    PieceType::None,
                    MoveFlag::Capture
                ));

                break;
            }

            // Own piece: bishop is blocked and cannot move further.
            else {
                break;
            }

            file += bishop_directions[dir][0];
            rank += bishop_directions[dir][1];
        }
    }
}



//ROOK

void generateRookMoves(const Position& pos, MoveList& moves, Square source, Piece rook) {
    int source_idx = static_cast<int>(source);

    int source_file = source_idx % 8;
    int source_rank = source_idx / 8;

    // Rook moves in 4 straight directions:
    // right, left, up, down.
    int rook_directions[4][2] = {
        { 1,  0},
        {-1,  0},
        { 0,  1},
        { 0, -1}
    };

    for (int dir = 0; dir < 4; dir++) {
        int file = source_file + rook_directions[dir][0];
        int rank = source_rank + rook_directions[dir][1];

        // Keep moving in this direction until blocked or outside board.
        while (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
            int target_idx = rank * 8 + file;
            Square target_square = static_cast<Square>(target_idx);

            Piece target_piece = pos.pieceAt(target_square);

            // Empty square: rook can move here and continue sliding.
            if (target_piece == Piece::None) {
                moves.push_back(Move(
                    source,
                    target_square,
                    rook,
                    Piece::None,
                    PieceType::None,
                    MoveFlag::Quiet
                ));
            }

            // Enemy piece: rook can capture it, but cannot move beyond it.
            else if (pieceColor(target_piece) == opposite(pos.sideToMove)) {
                moves.push_back(Move(
                    source,
                    target_square,
                    rook,
                    target_piece,
                    PieceType::None,
                    MoveFlag::Capture
                ));

                break;
            }

            // Own piece: rook is blocked.
            else {
                break;
            }

            file += rook_directions[dir][0];
            rank += rook_directions[dir][1];
        }
    }
}


// QUEEN

void generateQueenMoves(const Position& pos, MoveList& moves, Square source, Piece queen) {
    int source_idx = static_cast<int>(source);

    int source_file = source_idx % 8;
    int source_rank = source_idx / 8;

    // Queen moves in all 8 sliding directions:
    // rook directions + bishop directions.
    int queen_directions[8][2] = {
        { 1,  0},  // right
        {-1,  0},  // left
        { 0,  1},  // up
        { 0, -1},  // down

        { 1,  1},  // top-right
        {-1,  1},  // top-left
        { 1, -1},  // bottom-right
        {-1, -1}   // bottom-left
    };

    for (int dir = 0; dir < 8; dir++) {
        int file = source_file + queen_directions[dir][0];
        int rank = source_rank + queen_directions[dir][1];

        // Keep moving in this direction until blocked or outside the board.
        while (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
            int target_idx = rank * 8 + file;
            Square target_square = static_cast<Square>(target_idx);

            Piece target_piece = pos.pieceAt(target_square);

            // Empty square: queen can move here and continue sliding.
            if (target_piece == Piece::None) {
                moves.push_back(Move(
                    source,
                    target_square,
                    queen,
                    Piece::None,
                    PieceType::None,
                    MoveFlag::Quiet
                ));
            }

            // Enemy piece: queen can capture it, but cannot move beyond it.
            else if (pieceColor(target_piece) == opposite(pos.sideToMove)) {
                moves.push_back(Move(
                    source,
                    target_square,
                    queen,
                    target_piece,
                    PieceType::None,
                    MoveFlag::Capture
                ));

                break;
            }

            // Own piece: queen is blocked.
            else {
                break;
            }

            file += queen_directions[dir][0];
            rank += queen_directions[dir][1];
        }
    }
}


// KING

void generateKingMoves(const Position& pos, MoveList& moves, Square source, Piece king) {
    int source_idx = static_cast<int>(source);

    int source_file = source_idx % 8;
    int source_rank = source_idx / 8;

    // King moves one square in any of the 8 directions.
    int king_directions[8][2] = {
        { 1,  0},  // right
        {-1,  0},  // left
        { 0,  1},  // up
        { 0, -1},  // down

        { 1,  1},  // top-right
        {-1,  1},  // top-left
        { 1, -1},  // bottom-right
        {-1, -1}   // bottom-left
    };

    for (int dir = 0; dir < 8; dir++) {
        int target_file = source_file + king_directions[dir][0];
        int target_rank = source_rank + king_directions[dir][1];

        // Skip squares outside the board.
        if (target_file < 0 || target_file > 7 || target_rank < 0 || target_rank > 7) {
            continue;
        }

        int target_idx = target_rank * 8 + target_file;
        Square target_square = static_cast<Square>(target_idx);

        Piece target_piece = pos.pieceAt(target_square);

        // Empty square: normal quiet king move.
        if (target_piece == Piece::None) {
            moves.push_back(Move(
                source,
                target_square,
                king,
                Piece::None,
                PieceType::None,
                MoveFlag::Quiet
            ));
        }

        // Enemy piece: king can capture it in pseudo-legal generation.
        else if (pieceColor(target_piece) == opposite(pos.sideToMove)) {
            moves.push_back(Move(
                source,
                target_square,
                king,
                target_piece,
                PieceType::None,
                MoveFlag::Capture
            ));
        }

        // Own piece: blocked, no move.
    }
}

    void generatePseudoLegalMoves (const Position &pos, MoveList& moves){

        moves.clear();

        for (int square_idx = 0; square_idx < 64; square_idx++){
            Square square = static_cast <Square> (square_idx);
            Piece piece = pos.pieceAt(square); 
            if (piece == Piece::None) continue; // Continue for empty squares.
            if (pieceColor(piece) != pos.sideToMove) continue;  // Continue if square is occupied by opponent piece.

            PieceType piece_type = ReturnPieceType (piece);
            
            if (piece_type == PieceType :: Pawn) {
                generatePawnMoves (pos, moves, square, piece);
            }
            else if (piece_type == PieceType :: Knight){
                generateKnightMoves (pos, moves, square, piece);
            }
            else if (piece_type == PieceType :: Bishop){
                generateBishopMoves (pos, moves, square, piece);
            }
            else if (piece_type == PieceType :: Rook){
                generateRookMoves (pos, moves, square, piece);
            }
            else if (piece_type == PieceType :: Queen){
                generateQueenMoves (pos, moves, square, piece);
            }
            else if (piece_type == PieceType :: King){
                generateKingMoves (pos, moves, square, piece);
            }
        }
    }

}