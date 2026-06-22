// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
// This is a space separated string, containing 6 segments and separated by 5 spaces.
/*
placement  = rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
side to move = w
castling   = KQkq
enPassant  = -
halfmove   = 0
fullmove   = 1
*/

#include "fen.h" // Our header file, which also includes position.h

#include <sstream> // Needed for string parsing, we can separate the string using the 5 spaces with the help of this lib.
#include <cctype> // to use functions like is_digit(character) {Checks if a character is a valid int or not}

namespace BitKnight {

// Converts a FEN piece character into our Piece enum.
// static because the function is visible only inside this cpp file, it is just a private helper used by loadFEN.
static Piece charToPiece(char c) {
    switch (c) {
        case 'P': return Piece::WhitePawn;
        case 'N': return Piece::WhiteKnight;
        case 'B': return Piece::WhiteBishop;
        case 'R': return Piece::WhiteRook;
        case 'Q': return Piece::WhiteQueen;
        case 'K': return Piece::WhiteKing;

        case 'p': return Piece::BlackPawn;
        case 'n': return Piece::BlackKnight;
        case 'b': return Piece::BlackBishop;
        case 'r': return Piece::BlackRook;
        case 'q': return Piece::BlackQueen;
        case 'k': return Piece::BlackKing;

        default: return Piece::None;
    }
}

// Converts file/rank characters like 'e' and '4' into Square::E4.
// Used for the enpassant field, because it has things like e3.

static Square squareFromFileRank(char fileChar, char rankChar) {
    if (fileChar < 'a' || fileChar > 'h') {
        return Square::None;
    }

    if (rankChar < '1' || rankChar > '8') {
        return Square::None;
    }

    int file = fileChar - 'a';  // 'a' -> 0, 'b' -> 1, ..., 'h' -> 7
    int rank = rankChar - '1';  // '1' -> 0, '2' -> 1, ..., '8' -> 7

    int squareIndex = rank * 8 + file;
    return static_cast<Square>(squareIndex);
}

bool loadFEN(Position& pos, const std::string& fen) { // Pass pos by reference.
    std::istringstream str_stream(fen);
    // str_stream is an input-string-stream object we created, to parse the fen string.

    std::string placement;
    std::string side;
    std::string castling;
    std::string enPassant;
    std::string halfmove;
    std::string fullmove;

    // FEN must have exactly these six main fields.
    if (!(str_stream >> placement >> side >> castling >> enPassant >> halfmove >> fullmove)) {
        return false;
    }

    pos.clear();

    // FEN piece placement starts from rank 8 and goes down to rank 1.
    int rank = 7;
    int file = 0;

    for (char c : placement) { // In placement, there can only be /, digits or piece symbols.
        if (c == '/'){
            if (file != 8 || rank == 0) return false;
            rank--;
            file = 0;
            continue;
        }
        
        if (std :: isdigit(static_cast <unsigned char> (c))) { // subtract the file by the number written in
            int empty_squares = c - '0';
            file += empty_squares; 
            if (file > 8) return false;
            continue;
        }

        // Only case remaining now is if the character is a letter.

        if (file > 7) return false; // For the case of 9 consecutive letters bw 2 slashes.
        Piece piece = charToPiece (c);
        if (piece == Piece :: None) return false;

        int square_index = rank * 8 + file;
        Square square = static_cast <Square> (square_index);
        pos.setPiece (piece, square);
        file++;
    
    }

    // After reading placement, we should end exactly at rank 1, file h.
    if (rank != 0 || file != 8) {
        return false;
    }

    if (side == "w") {
        pos.sideToMove = Color::White;
    } else if (side == "b") {
        pos.sideToMove = Color::Black;
    } else {
        return false;
    }

    // Castling rights are stored as bit flags:
    // 1 = White kingside, 2 = White queenside, 4 = Black kingside, 8 = Black queenside.
   
// pos.clear() already reset castlingRights to 0.

    if (castling != "-") {
        for (char c : castling) {
            switch (c) {
                case 'K':
                    pos.castlingRights |= 1;
                    break;
                case 'Q':
                    pos.castlingRights |= 2;
                    break;
                case 'k':
                    pos.castlingRights |= 4;
                    break;
                case 'q':
                    pos.castlingRights |= 8;
                    break;
                default:
                    return false;
            }
        }
    }

    if (enPassant == "-") {
        pos.enPassantSquare = Square::None;  // This is by default initialized in the constructor, still we are writing here for clarity again.
    } else {
        if (enPassant.size() != 2) {
            return false;
        }
        char enPassant_file = enPassant[0];
        char enPassant_rank = enPassant[1];
        pos.enPassantSquare = squareFromFileRank(enPassant_file, enPassant_rank);

        if (pos.enPassantSquare == Square::None) {
            return false; // None is returned if either of enPassant_file or enPassant_rank were invalid/out of bounds so returned None.
        }
        if ((enPassant_rank != '3' && enPassant_rank != '6') == true) return false; // EnPassant can only happen at the 3 or the 6th rank.
    }

    // Convert halfmove/fullmove fields from string to integer.
    try {
        pos.halfmoveClock = std::stoi(halfmove);
        pos.fullmoveNumber = std::stoi(fullmove);
    } catch (...) { // (...) catches any exception.
        return false;
    }

    if (pos.halfmoveClock < 0 || pos.fullmoveNumber <= 0) {
        return false;
    }
// Every time a pawn moves or capture happens, halfmove resets to 0, so 0 is valid for it. FullMoveNumber always begins with 1.
    return true;
}

}