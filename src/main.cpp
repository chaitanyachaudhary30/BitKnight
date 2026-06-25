#include <iostream>
#include <string>

#include "fen.h"
#include "make_move.h"

int main() {
    using namespace std;
    using namespace BitKnight;

    cout << boolalpha;

    // ------------------------------------------------------------
    // Test 1: Double pawn push e2 -> e4
    // ------------------------------------------------------------
    {
        Position pos;

        if (!loadFEN(pos, STARTING_FEN)) {
            cout << "Test 1: Failed to load starting FEN.\n";
            return 1;
        }

        Move move(
            Square::E2,
            Square::E4,
            Piece::WhitePawn,
            Piece::None,
            PieceType::None,
            MoveFlag::DoublePawnPush
        );

        bool ok = makeMove(pos, move);

        cout << "\nTest 1: Double pawn push e2 -> e4\n";
        pos.print();

        cout << "makeMove returned: " << ok << "\n";
        cout << "E4 has WhitePawn: " << (pos.pieceAt(Square::E4) == Piece::WhitePawn) << "\n";
        cout << "E2 is empty: " << (pos.pieceAt(Square::E2) == Piece::None) << "\n";
        cout << "En passant square is E3: " << (pos.enPassantSquare == Square::E3) << "\n";
        cout << "Side to move is Black: " << (pos.sideToMove == Color::Black) << "\n";
    }

    // ------------------------------------------------------------
    // Test 2: Normal capture e4xd5
    // ------------------------------------------------------------
    {
        Position pos;

        const string TEST_FEN =
            "4k3/8/8/3p4/4P3/8/8/4K3 w - - 0 1";

        if (!loadFEN(pos, TEST_FEN)) {
            cout << "Test 2: Failed to load FEN.\n";
            return 1;
        }

        Move move(
            Square::E4,
            Square::D5,
            Piece::WhitePawn,
            Piece::BlackPawn,
            PieceType::None,
            MoveFlag::Capture
        );

        bool ok = makeMove(pos, move);

        cout << "\nTest 2: Normal capture e4xd5\n";
        pos.print();

        cout << "makeMove returned: " << ok << "\n";
        cout << "D5 has WhitePawn: " << (pos.pieceAt(Square::D5) == Piece::WhitePawn) << "\n";
        cout << "E4 is empty: " << (pos.pieceAt(Square::E4) == Piece::None) << "\n";
        cout << "Halfmove clock reset: " << (pos.halfmoveClock == 0) << "\n";
    }

    // ------------------------------------------------------------
    // Test 3: Promotion e7 -> e8 = Queen
    // ------------------------------------------------------------
    {
        Position pos;

        const string TEST_FEN =
            "k7/4P3/8/8/8/8/8/4K3 w - - 0 1";

        if (!loadFEN(pos, TEST_FEN)) {
            cout << "Test 3: Failed to load FEN.\n";
            return 1;
        }

        Move move(
            Square::E7,
            Square::E8,
            Piece::WhitePawn,
            Piece::None,
            PieceType::Queen,
            MoveFlag::Promotion
        );

        bool ok = makeMove(pos, move);

        cout << "\nTest 3: Promotion e7 -> e8 = Queen\n";
        pos.print();

        cout << "makeMove returned: " << ok << "\n";
        cout << "E8 has WhiteQueen: " << (pos.pieceAt(Square::E8) == Piece::WhiteQueen) << "\n";
        cout << "E7 is empty: " << (pos.pieceAt(Square::E7) == Piece::None) << "\n";
    }

    // ------------------------------------------------------------
    // Test 4: White kingside castling e1 -> g1
    // ------------------------------------------------------------
    {
        Position pos;

        const string TEST_FEN =
            "4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1";

        if (!loadFEN(pos, TEST_FEN)) {
            cout << "Test 4: Failed to load FEN.\n";
            return 1;
        }

        Move move(
            Square::E1,
            Square::G1,
            Piece::WhiteKing,
            Piece::None,
            PieceType::None,
            MoveFlag::KingCastle
        );

        bool ok = makeMove(pos, move);

        cout << "\nTest 4: White kingside castling\n";
        pos.print();

        cout << "makeMove returned: " << ok << "\n";
        cout << "G1 has WhiteKing: " << (pos.pieceAt(Square::G1) == Piece::WhiteKing) << "\n";
        cout << "F1 has WhiteRook: " << (pos.pieceAt(Square::F1) == Piece::WhiteRook) << "\n";
        cout << "E1 is empty: " << (pos.pieceAt(Square::E1) == Piece::None) << "\n";
        cout << "H1 is empty: " << (pos.pieceAt(Square::H1) == Piece::None) << "\n";
        cout << "White castling rights removed: " << ((pos.castlingRights & 1) == 0 && (pos.castlingRights & 2) == 0) << "\n";
    }

    // ------------------------------------------------------------
    // Test 5: En passant e5xd6
    // ------------------------------------------------------------
    {
        Position pos;

        const string TEST_FEN =
            "4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 1";

        if (!loadFEN(pos, TEST_FEN)) {
            cout << "Test 5: Failed to load FEN.\n";
            return 1;
        }

        Move move(
            Square::E5,
            Square::D6,
            Piece::WhitePawn,
            Piece::BlackPawn,
            PieceType::None,
            MoveFlag::EnPassant
        );

        bool ok = makeMove(pos, move);

        cout << "\nTest 5: En passant e5xd6\n";
        pos.print();

        cout << "makeMove returned: " << ok << "\n";
        cout << "D6 has WhitePawn: " << (pos.pieceAt(Square::D6) == Piece::WhitePawn) << "\n";
        cout << "E5 is empty: " << (pos.pieceAt(Square::E5) == Piece::None) << "\n";
        cout << "D5 is empty: " << (pos.pieceAt(Square::D5) == Piece::None) << "\n";
        cout << "En passant square cleared: " << (pos.enPassantSquare == Square::None) << "\n";
    }

    return 0;
}