#pragma once
#include "chess_basics.hpp"
#include "primitives.hpp"

std::array<std::array<wchar_t, 6>, 3> pieceAscii{{
    {'♟', '♜', '♞', '♝', '♛', '♚'},
    {'♙', '♖', '♘', '♗', '♕', '♔'},
    {'▢', '▢', '▢', '▢', '▢', '▢'}
}};
/*
std::unordered_map<Piece, wchar_t> pieceAsciiMap {[&pieceAscii]{
    std::unordered_map<Piece, wchar_t> pieceAsciiMap;
    for (int color{}; color < 3; color++) for (int piece{}; piece < 6; piece++) {
        pieceAsciiMap[Piece{color, piece}] = pieceAscii[color][piece];
    }
    return pieceAsciiMap;
}()
};
*/
class fout_stream {
    template <typename T> fout_stream& operator<< (T);
} fancy_display;

template <typename T> fout_stream& fout_stream::operator<< (T obj) {
    std::cout << obj;
    return *this;
} 

template <> fout_stream& fout_stream::operator<< <Piece> (Piece chessPiece) {
    std::cout << std::array<char, 2>{'W', 'B'}[chessPiece.color]
        << std::array<char, 6>{'P', 'R', 'K', 'B', 'Q', 'K'}[chessPiece.type];
    return *this;
}

template <> fout_stream& fout_stream::operator<< <ChessGame> (ChessGame cg) {
    std::cout << cg.getTurn();
    for (const auto& row: cg.getBoard()) {
       for (const auto& val: row) {
           std::cout << pieceAscii[val.color][val.type];
       }
    }

    return *this
}

template <> fout_stream& fout_stream::operator<< <ChessMove> (ChessMove cm) {
    switch (cm.moveType) {
        case 0:
            std::cout << '(' << cm.start.x << ',' << cm.start.y << ')'
                << '(' << cm.end.x << ',' << cm.end.y << ')';
            break;
        case 1:
            std::cout << (cm.color == 0 ? 'W' : 'B')
                << (cm.castleType == 0 ? "O-O" : "O-O-O");
            break;
        case 2:
            std::cout << (cm.color == 0 ? 'w')
    }
}

/*
template <typename T> fancy_display& operator<<(T display_type, const Piece chessPiece) {
    out << std::array<char, 2>{'W', 'B'}[chessPiece.color]
        << std::array<char, 6>{'P', 'R', 'K', 'B', 'Q', 'K'}[chessPiece.type];
    return out;
}

template <typename T> fancy_display& operator<<(std::ostream& out, std::vector<T>) {
    out << "[ ";
    for (const auto& element: T) {
        out << element << ", ";
    }
    out << ']';
    return out;
}
*/

