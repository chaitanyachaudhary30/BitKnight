// This does not decide whether the move is fully legal. It only applies the move if the move is structurally consistent.
/* MOVE EXECUTION PIPELINE:
validate move 
↓
record old-state information
↓
update rights that depend on old squares
↓
remove captured pieces
↓
move the main piece
↓
move rook if castling
↓
update en passant square
↓
update clocks
↓
switch side
↓
refresh occupancies
*/


/* REASON FOR ORDER OF THE FUNCTIONS: 
targetPiece before moving         -> because destination info is old-board info
updateCastlingRights before move  ->  because rights depend on from/to/captured old-state info
remove captures before placing    ->  because destination may be occupied
promotion handling during placing -> because pawn becomes another piece
rook movement after king move     -> because castling moves two pieces
enPassantSquare after move        -> because it is caused by the move
sideToMove near end               -> because everything before uses moving side
occupancies last                  -> because all piece changes are done
*/


    #include "make_move.h"

    namespace BitKnight {

    static Piece makePiece(Color color, PieceType type) {
        if (color == Color::None || type == PieceType::None) {
            return Piece::None;
        }

        int base = (color == Color::White) ? 0 : 6;
        int typeValue = static_cast<int>(type);

        return static_cast<Piece>(base + typeValue);
    }

    static bool isPawn(Piece piece) {
        return piece == Piece::WhitePawn || piece == Piece::BlackPawn;
    }

    static void updateCastlingRights(Position& pos, const Move& move) {
        // Castling encoding:
        // 1 = White kingside
        // 2 = White queenside
        // 4 = Black kingside
        // 8 = Black queenside

        // If white king moves, White loses both castling rights.
        if (move.piece == Piece::WhiteKing) {
            pos.castlingRights &= ~1;
            pos.castlingRights &= ~2;
        }

        // If black king moves, Black loses both castling rights.
        if (move.piece == Piece::BlackKing) {
            pos.castlingRights &= ~4;
            pos.castlingRights &= ~8;
        }

        // If white rook moves from h1/a1, remove that side's castling right.
        if (move.piece == Piece::WhiteRook) {
            if (move.from == Square::H1) {
                pos.castlingRights &= ~1;
            } else if (move.from == Square::A1) {
                pos.castlingRights &= ~2;
            }
        }

        // If black rook moves from h8/a8, remove that side's castling right.
        if (move.piece == Piece::BlackRook) {
            if (move.from == Square::H8) {
                pos.castlingRights &= ~4;
            } else if (move.from == Square::A8) {
                pos.castlingRights &= ~8;
            }
        }

        // If a rook is captured on its original square, remove that castling right.
        if (move.captured == Piece::WhiteRook) {
            if (move.to == Square::H1) {
                pos.castlingRights &= ~1;
            } else if (move.to == Square::A1) {
                pos.castlingRights &= ~2;
            }
        }

        if (move.captured == Piece::BlackRook) {
            if (move.to == Square::H8) {
                pos.castlingRights &= ~4;
            } else if (move.to == Square::A8) {
                pos.castlingRights &= ~8;
            }
        }
    }

    // If DoublePawnPush, set enPassantSquare to the square behind the pawn.
    static void updateEnPassantSquare(Position& pos, const Move& move) {

        if (move.flag == MoveFlag :: DoublePawnPush) {
            int enum_double_pawn = static_cast <int> (move.to);
            int enum_enPassant = (move.piece == Piece :: WhitePawn) ? enum_double_pawn-8 : enum_double_pawn+8;
            pos.enPassantSquare = static_cast <Square> (enum_enPassant); 
        }
        else pos.enPassantSquare = Square :: None;
    }

    static void updateMoveCounters(Position& pos, const Move& move, Color movingColor) {
        // 50-move clock resets after pawn move or any capture.
        if (isPawn(move.piece) || isCapture(move)) {
            pos.halfmoveClock = 0;
        } else {
            pos.halfmoveClock++;
        }

        // Fullmove number increments after Black makes a move.
        if (movingColor == Color::Black) {
            pos.fullmoveNumber++;
        }
    }

    bool makeMove(Position& pos, const Move& move) {
        if (move.from == Square::None ||
            move.to == Square::None ||
            move.piece == Piece::None) {
            return false;
        }

        Color movingColor = pieceColor(move.piece);

        if (movingColor == Color::None) {
            return false;
        }

        // The piece on the source square must match the move's moving piece.
        if (pos.pieceAt(move.from) != move.piece) {
            return false;
        }

        // Make sure the move belongs to the side whose turn it is.
        if (movingColor != pos.sideToMove) {
            return false;
        }

        Piece targetPiece = pos.pieceAt(move.to); // Piece currently on destination square.
        
        // Validate normal captures (not EnPassant).
        if (move.flag == MoveFlag::Capture ||
            move.flag == MoveFlag::PromotionCapture) {
            if (move.captured == Piece::None) return false;
            
            if (targetPiece == Piece::None) {
                return false;
            }

            if (targetPiece != move.captured) {
                return false;
            }

            if (pieceColor(targetPiece) == movingColor) {
                return false;
            }
        }
        // Now validate EnPassant
        else if (move.flag == MoveFlag::EnPassant) {
            if (targetPiece != Piece::None) {
                return false;
            }
            if (move.captured == Piece::None) return false;

            int toIndex = static_cast<int>(move.to);

            int capturedIndex =
                (movingColor == Color::White) ? toIndex - 8 : toIndex + 8;

            if (capturedIndex < 0 || capturedIndex >= 64) {
                return false;
            }

            Square capturedSquare = static_cast<Square>(capturedIndex);

            Piece expectedCapturedPawn =
                (movingColor == Color::White) ? Piece::BlackPawn : Piece::WhitePawn;

            if (pos.pieceAt(capturedSquare) != expectedCapturedPawn) {
                return false;
            }

            if (move.captured != expectedCapturedPawn) { // Added this while debugging, important edgecase.
                return false;
            }
        }
        else if (move.captured != Piece::None) return false;


        // Validate quiet/non-capture moves.
        if (move.flag == MoveFlag::Quiet ||
            move.flag == MoveFlag::DoublePawnPush ||
            move.flag == MoveFlag::Promotion ||
            move.flag == MoveFlag::KingCastle ||
            move.flag == MoveFlag::QueenCastle) {
            if (targetPiece != Piece::None) {
                return false;
            }
        }

        // Validate promotion consistency.
        if (isPromotion(move)) {
            if (move.promotion == PieceType::None ||
                move.promotion == PieceType::Pawn ||
                move.promotion == PieceType::King) {
                return false;
            }
        } 
        else {
            if (move.promotion != PieceType::None) {
                return false;
            }
        }


        // Validate castling rook existence.
        if (move.flag == MoveFlag::KingCastle) {
            if (movingColor == Color::White) {
                if (move.from != Square::E1 || move.to != Square::G1 ||
                    pos.pieceAt(Square::H1) != Piece::WhiteRook) {
                    return false;
                }
            } else {
                if (move.from != Square::E8 || move.to != Square::G8 ||
                    pos.pieceAt(Square::H8) != Piece::BlackRook) {
                    return false;
                }
            }
        }

        if (move.flag == MoveFlag::QueenCastle) {
            if (movingColor == Color::White) {
                if (move.from != Square::E1 || move.to != Square::C1 ||
                    pos.pieceAt(Square::A1) != Piece::WhiteRook) {
                    return false;
                }
            } else {
                if (move.from != Square::E8 || move.to != Square::C8 ||
                    pos.pieceAt(Square::A8) != Piece::BlackRook) {
                    return false;
                }
            }
        }

        // Update castling rights before pieces are moved.
        updateCastlingRights(pos, move);

        // Handle captures.
        if (move.flag == MoveFlag::EnPassant) {
            int toIndex = static_cast<int>(move.to);

            int capturedIndex =
                (movingColor == Color::White) ? toIndex - 8 : toIndex + 8;

            Square capturedSquare = static_cast<Square>(capturedIndex);

            Piece capturedPawn =
                (movingColor == Color::White) ? Piece::BlackPawn : Piece::WhitePawn;

            pos.removePiece(capturedPawn, capturedSquare);
        } else if (move.flag == MoveFlag::Capture ||
                move.flag == MoveFlag::PromotionCapture) {
            pos.removePiece(move.captured, move.to);
        }

        // Move the main piece.
        pos.removePiece(move.piece, move.from);

        if (isPromotion(move)) {
            Piece promotedPiece = makePiece(movingColor, move.promotion);

            if (promotedPiece == Piece::None) {
                return false;
            }

            pos.setPiece(promotedPiece, move.to);
        } else {
            pos.setPiece(move.piece, move.to);
        }

        // Handle rook movement during castling.
        if (move.flag == MoveFlag::KingCastle) {
            if (movingColor == Color::White) {
                pos.removePiece(Piece::WhiteRook, Square::H1);
                pos.setPiece(Piece::WhiteRook, Square::F1);
            } else {
                pos.removePiece(Piece::BlackRook, Square::H8);
                pos.setPiece(Piece::BlackRook, Square::F8);
            }
        } else if (move.flag == MoveFlag::QueenCastle) {
            if (movingColor == Color::White) {
                pos.removePiece(Piece::WhiteRook, Square::A1);
                pos.setPiece(Piece::WhiteRook, Square::D1);
            } else {
                pos.removePiece(Piece::BlackRook, Square::A8);
                pos.setPiece(Piece::BlackRook, Square::D8);
            }
        }

        updateEnPassantSquare(pos, move);

        updateMoveCounters(pos, move, movingColor);

        pos.sideToMove = opposite(pos.sideToMove);

        pos.updateOccupancies();

        return true;
    }

    }


