    #include "legal_moves.h"

    #include "make_move.h"
    #include "attack.h"
    /*
    1. Clear legalMoves.
    2. Generate pseudo-legal moves.
    3. For every pseudo-legal move:
        a. Remember original side to move.
        b. If move is castling, check:
                - king is not currently in check
                - king does not pass through an attacked square
        c. Copy the position.
        d. makeMove(copy, move)
        e. If original side's king is NOT in check after the move:
                keep the move.
    */
    namespace BitKnight {

        void generateLegalMoves (const Position &pos, MoveList& legalMoves) {
            legalMoves.clear();
            MoveList pseudo_legal_moves;
            Color ToMove = pos.sideToMove;
            generatePseudoLegalMoves (pos, pseudo_legal_moves);
            
            for (auto move : pseudo_legal_moves) {
                Position copy = pos;
// Below line is only a safeguard. If the generatePseudoLegalMoves function is bug-free, it will always pass.
                if (!makeMove (copy, move)) continue; 

                if (isCastle(move)){
        // We have already checked that all squares in bw are empty,
        // just check now whether the king was originally in check, and whether the squares in bw were in check.
                    
        // Pass pos in the below funtion, not copy.
        // Because we want the original position of the king to be safe.
                    if (isKingInCheck(pos, ToMove)) continue; 
                    if (move.flag == MoveFlag::KingCastle) {
                        if (ToMove == Color::White){
                            if (isSquareAttacked(copy, Square::F1, opposite(ToMove)) || isSquareAttacked(copy, Square::G1, opposite(ToMove))) continue;
                        } 

                        else if (isSquareAttacked(copy, Square::F8, opposite(ToMove)) || isSquareAttacked(copy, Square::G8, opposite(ToMove))) continue;
                    }  

                    else {
                        if (ToMove == Color::White){
                            if (isSquareAttacked(copy, Square::D1, opposite(ToMove)) || isSquareAttacked(copy, Square::C1, opposite(ToMove))) continue;
                        }

                        else if (isSquareAttacked(copy, Square::D8, opposite(ToMove)) || isSquareAttacked(copy, Square::C8, opposite(ToMove))) continue;
                    }  

        // If not continued till now, means the castling was valid, so add the move.
                    legalMoves.push_back(move);
                }

            // Now storing ToMove will prove useful.
                else if (!isKingInCheck(copy, ToMove)){
                    legalMoves.push_back(move);
                }
            }
        }
    }