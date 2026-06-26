#include <iostream>
#include <string>

#include "fen.h"
#include "move_gen.h"
#include "legal_moves.h"
#include "attack.h"

int main() {
    using namespace std;
    using namespace BitKnight;

    cout << boolalpha;

    Position pos;

    const string TEST_FEN =
        "k3r3/8/8/8/8/8/8/4K3 w - - 0 1";

    if (!loadFEN(pos, TEST_FEN)) {
        cout << "Failed to load test FEN.\n";
        return 1;
    }

    pos.print();

    cout << "White king in check: "
         << isKingInCheck(pos, Color::White) << "\n";

    MoveList pseudoMoves;
    generatePseudoLegalMoves(pos, pseudoMoves);

    MoveList legalMoves;
    generateLegalMoves(pos, legalMoves);

    cout << "Pseudo-legal moves: " << pseudoMoves.size() << "\n";
    cout << "Legal moves: " << legalMoves.size() << "\n";

    cout << "\nExpected idea:\n";
    cout << "Pseudo-legal king moves may include unsafe squares.\n";
    cout << "Legal moves should remove moves that leave White in check.\n";

    return 0;
}