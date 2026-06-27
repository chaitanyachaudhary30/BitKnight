#include <iostream>
#include <string>

#include "UCI.h"
#include "fen.h"
#include "perft.h"

int main(int argcnt, char* argarr[]) { // argcnt is count of command line arguments.
    // We can't write string argarr, its not allwowed for command line.
    using namespace std;
    using namespace BitKnight;

    /*
        Command-line modes:

        1. Normal:
            BitKnight.exe

            Starts UCI mode, used by Cute Chess / Arena.

        2. Perft:
            BitKnight.exe perft 3

            Runs perft on starting position at depth 3.

        3. Divide:
            BitKnight.exe divide 2

            Prints each root move and its perft count.

        4. perftfen: 
            Runs Perft test on custom FENs.
    */
    if (argcnt >= 2) {  // argcnt = 3 for mode = perft, and divide.
        string mode = argarr[1];

        if (mode == "perft" || mode == "divide") {
            int depth = 1; // default depth, if they don't enter depth then this will be considered default.

            if (argcnt == 3) {
                depth = stoi(argarr[2]);
            }

            Position pos;

            if (!loadFEN(pos, STARTING_FEN)) {
                cout << "Failed to load starting position.\n";
                return 1;
            }

            if (mode == "perft") {
                cout << perft(pos, depth) << "\n";
            } else {
                perftDivide(pos, depth);
            }
            return 0;
        }

        else if (mode == "perftfen"){ // argcnt == 4
            int depth = 1;
            string curFEN = argarr[2];
            if (argcnt == 4) {
                depth = stoi(argarr[2]);
                curFEN = argarr[3];

            }
            Position pos;
            if (!loadFEN(pos, curFEN)) {
                cout << "Failed to load starting position.\n";
                return 1;
            }        
            for (int level = 1; level <= depth; level++) {
                cout << "Depth " << level << ": " << perft(pos, level) << endl;
            }    
            return 0;
        }
    }

    /*
        If no command-line argument is given, BitKnight behaves like a UCI engine.

        This is the mode used by Cute Chess.
    */
    runUciLoop();

    return 0;
}

/*
    PERFT TEST for 6 random diverse positions, after having passed for the Starting position upto depth 6.
    Source: Chessprogramming Wiki perft test positions.
    Format:
        TEST_NAME | FEN | Expected: D1 D2 D3 D4 D5

    1) KIWIPETE
       FEN: r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1
       Expected: D1=48 D2=2039 D3=97862 D4=4085603 D5=193690690

    2) ENDGAME / ROOK-PAWN POSITION
       FEN: 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1
       Expected: D1=14 D2=191 D3=2812 D4=43238 D5=674624

    3) PROMOTION / TACTICAL POSITION
       FEN: r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1
       Expected: D1=6 D2=264 D3=9467 D4=422333 D5=15833292

    4) MIRRORED PROMOTION / TACTICAL POSITION
       FEN: r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1
       Expected: D1=6 D2=264 D3=9467 D4=422333 D5=15833292

    5) BUG-CATCHER POSITION
       FEN: rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8
       Expected: D1=44 D2=1486 D3=62379 D4=2103487 D5=89941194

    6) COMPLEX MIDDLEGAME POSITION
       FEN: r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10
       Expected: D1=46 D2=2079 D3=89890 D4=3894594 D5=164075551
*/