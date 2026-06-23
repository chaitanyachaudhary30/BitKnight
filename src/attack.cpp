#include "attack.h"

namespace BitKnight {

    static const PieceType ReturnPieceType (Piece piece){
        if (piece == Piece :: None) return PieceType :: None;
        int enum_value = (static_cast <int> (piece))%6;
        return static_cast<PieceType> (enum_value);
    }


Square findKing(const Position& pos, Color color) {
    Piece king = Piece :: WhiteKing;
    if (color == Color :: Black) king : Piece::BlackKing;

    for (int squareIdx = 0; squareIdx < 64; squareIdx++) {
        Square square = static_cast<Square>(squareIdx);

        if (pos.pieceAt(square) == king) {
            return square;
        }
    }

    return Square::None;
}

bool isSquareAttacked(const Position& pos, Square square, Color attackingColor) {
    if (square == Square::None || attackingColor == Color::None) {
        return false;
    }
    if (attackingColor == pos.sideToMove) return false;

    int squareIdx = static_cast<int>(square);
    int file = squareIdx % 8;
    int rank = squareIdx / 8;

    /*
        Pawn attacks.

        Important:
        We are checking if 'square' is attacked.
        So we look backwards from the target square to find possible attacking pawns.
    */
    if (attackingColor == Color::White) {

        // A white pawn attacks upward using +7 or +9.
        // Therefore, from the target square, possible white pawn sources are -7 and -9.
        if (file < 7) {// capture towards left possible, direction = -9. Since file is of the piece about to be captured.
            int pawnIdx = squareIdx - 7;
            if (pawnIdx >= 0 && pawnIdx < 64) {
                Square pawnSquare = static_cast <Square> (pawnIdx); 
                    if (pos.pieceAt(pawnSquare) == Piece :: WhitePawn) return true;
            }
        }

        if (file > 0) {
            int pawnIdx = squareIdx - 9;

            if (pawnIdx >= 0 && pawnIdx < 64) {
                Square pawnSquare = static_cast<Square>(pawnIdx);

                if (pos.pieceAt(pawnSquare) == Piece::WhitePawn) {
                    return true;
                }
            }
        }
    } else if (attackingColor == Color::Black) {
        // A black pawn attacks downward using -7 or -9.
        // Therefore, from the target square, possible black pawn sources are +7 and +9.

        if (file > 0) {
            int sourceIdx = squareIdx + 7;

            if (sourceIdx >= 0 && sourceIdx < 64) {
                Square source = static_cast<Square>(sourceIdx);

                if (pos.pieceAt(source) == Piece::BlackPawn) {
                    return true;
                }
            }
        }

        if (file < 7) {
            int sourceIdx = squareIdx + 9;

            if (sourceIdx >= 0 && sourceIdx < 64) {
                Square source = static_cast<Square>(sourceIdx);

                if (pos.pieceAt(source) == Piece::BlackPawn) {
                    return true;
                }
            }
        }
    }

    /*
        Knight attacks.
        If a knight can move from A to B, it can also move from B to A.
    */
    int knightDirections[8][2] = {
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
        int knightFile = file + knightDirections[i][0];
        int knightRank = rank + knightDirections[i][1];

        if (knightFile < 0 || knightFile > 7 || knightRank < 0 || knightRank > 7) {
            continue;
        }

        int knightIdx = knightRank * 8 + knightFile;
        Square KnightSquare = static_cast<Square>(knightIdx);

        if (attackingColor == Color :: White && pos.pieceAt(KnightSquare) == Piece :: WhiteKnight) return true;
        if (attackingColor == Color :: Black && pos.pieceAt(KnightSquare) == Piece :: BlackKnight) return true;
    }


    /*
        // Bishop / Queen diagonal attacks.
        // Move outward from the target square diagonally.
        // The first piece found in each direction decides whether that direction attacks the square.
    */
    int bishopDirections[4][2] = {
        { 1,  1},
        {-1,  1},
        { 1, -1},
        {-1, -1}
    };

    for (int dir = 0; dir < 4; dir++) {
        int currentFile = file + bishopDirections[dir][0];
        int currentRank = rank + bishopDirections[dir][1];

        while (currentFile >= 0 && currentFile < 8 &&
               currentRank >= 0 && currentRank < 8) {

            int BiQueenIdx = currentRank * 8 + currentFile;
            Square BiQueenSquare = static_cast<Square>(BiQueenIdx); 

            Piece piece = pos.pieceAt(BiQueenSquare);

            if (piece != Piece::None) {
                if (pieceColor(piece) == attackingColor) {
                    PieceType type = ReturnPieceType(piece);

                    if (type == PieceType::Bishop || type == PieceType::Queen) {
                        return true;
                    }
                }

                // Any piece blocks further attack along this ray.
                else break;
            }

            currentFile += bishopDirections[dir][0];
            currentRank += bishopDirections[dir][1];
        }
    }

    
    // Rook / Queen straight-line attacks.

    int rookDirections[4][2] = {
        { 1,  0},
        {-1,  0},
        { 0,  1},
        { 0, -1}
    };

    for (int dir = 0; dir < 4; dir++) {
        int currentFile = file + rookDirections[dir][0];
        int currentRank = rank + rookDirections[dir][1];

        while (currentFile >= 0 && currentFile < 8 &&
               currentRank >= 0 && currentRank < 8) {

            int RoQueenIdx = currentRank * 8 + currentFile;
            Square RoQueenSquare = static_cast<Square>(RoQueenIdx);

            Piece piece = pos.pieceAt(RoQueenSquare);

            if (piece != Piece::None) {
                if (pieceColor(piece) == attackingColor) {
                    PieceType type = ReturnPieceType(piece);

                    if (type == PieceType::Rook || type == PieceType::Queen) {
                        return true;
                    }
                }

                // Any piece blocks further attack along this ray.
                break;
            }

            currentFile += rookDirections[dir][0];
            currentRank += rookDirections[dir][1];
        }
    }

       
    // King attacks.

    int kingDirections[8][2] = {
        { 1,  0},
        {-1,  0},
        { 0,  1},
        { 0, -1},
        { 1,  1},
        {-1,  1},
        { 1, -1},
        {-1, -1}
    };

    for (int i = 0; i < 8; i++) {
        int kingFile = file + kingDirections[i][0];
        int kingRank = rank + kingDirections[i][1];

        if (kingFile < 0 || kingFile > 7 || kingRank < 0 || kingRank > 7) {
            continue;
        }

        int kingIdx = kingRank * 8 + kingFile;
        Square kingSquare = static_cast<Square>(kingIdx);

        if (pos.pieceAt(kingSquare) == Piece :: WhiteKing && attackingColor == Color :: White) return true;
        if (pos.pieceAt(kingSquare) == Piece :: BlackKing && attackingColor == Color :: Black) return true;

    }

    return false;
}

bool isKingInCheck(const Position& pos, Color color) {
    Square kingSquare = findKing(pos, color);

    if (kingSquare == Square::None) {
        return false;
    }

    return isSquareAttacked(pos, kingSquare, opposite(color));
}

}