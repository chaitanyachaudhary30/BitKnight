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

        }
    }

}