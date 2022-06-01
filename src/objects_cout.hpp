#pragma once
#include "chess_basics.hpp"
#include "primitives.hpp"

std::array<std::array<wchar_t, 6>, 2> pieceAscii{{
    {'♟', '♜', '♞', '♝', '♛', '♚'},
    {'♙', '♖', '♘', '♗', '♕', '♔'}
}};

class : public std::ostream {} fancy_display;

std::ostream& operator<<(std::ostream& out, const Piece chessPiece) {
    out << std::array<char, 2>{'W', 'B'}[chessPiece.color]
        << std::array<char, 6>{'P', 'R', 'K', 'B', 'Q', 'K'}[chessPiece.type];
    return out;
}

template <typename T> std::ostream& operator<<(std::ostream& out, std::vector<T>) {
    out << "[ ";
    for (const auto& element: T) {
        out << element << ", ";
    }
    out << ']';
    return out;
}


