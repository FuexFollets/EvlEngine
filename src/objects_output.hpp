#pragma once
#include "chess_basics.hpp"
#include "primitives.hpp"

std::array<std::array<wchar_t, 6>, 2> pieceAscii{{
    {'♟', '♜', '♞', '♝', '♛', '♚'},
    {'♙', '♖', '♘', '♗', '♕', '♔'}
}};

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
    for (const auto&& row: cg.getBoard()) {
        
    }

    return *this
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

