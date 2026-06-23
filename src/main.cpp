#include <iostream>

#include "fen.h"
#include "attack.h"

int main() {
    using namespace std;
    using namespace BitKnight;

    Position pos;

    const string TEST_FEN =
        "k3r3/8/8/8/8/8/8/4K3 w - - 0 1";

    if (!loadFEN(pos, TEST_FEN)) {
        cout << "Failed to load test FEN.\n";
        return 1;
    }

    pos.print();

    cout << boolalpha;

    cout << "Is E1 attacked by Black? "
         << isSquareAttacked(pos, Square::E1, Color::Black) << "\n";

    cout << "Is White king in check? "
         << isKingInCheck(pos, Color::White) << "\n";

    cout << "Is Black king in check? "
         << isKingInCheck(pos, Color::Black) << "\n";

    cout << "\nExpected:\n";
    cout << "Is E1 attacked by Black? true\n";
    cout << "Is White king in check? true\n";
    cout << "Is Black king in check? false\n";

    return 0;
}