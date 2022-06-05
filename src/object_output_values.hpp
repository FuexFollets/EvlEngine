#pragma once
#include "primitives.hpp"


struct pl__ {
    const std::array<std::array<char, 6>, 2> ascii_vals{{
        {'P', 'N', 'B', 'R', 'K', 'Q'},
        {'p', 'n', 'b', 'r', 'k', 'q'}
    }};

    char operator[](Piece);
    template <typename T1> char operator[](T1);
} piece_letter;

char pl__::operator[](Piece piece) {
    return ascii_vals[piece.color][piece.type];
}

template <typename T1> char pl__::operator[](T1 row_ax) {
    return ascii_vals[row_ax];
}

const std::unordered_map<char, Piece> letter_to_piece{[]{
    const std::array<std::array<char, 6>, 2> ascii_vals{{
        {'P', 'N', 'B', 'R', 'K', 'Q'},
        {'p', 'n', 'b', 'r', 'k', 'q'}
    }};
    std::unordered_map<char, Piece> umap;
    uint8_t color{}, type{};
    for (const auto& row : ascii_vals) {
        for (const auto& letter : row) {
            umap[letter] = Piece{color, type++};
        }
        ++color;
    }
    return umap;
}()};

struct ac__ {
    std::string operator[](Cordinate);
} algebraic_cord;

std::string ac__::operator[](Cordinate cord) {
    std::string pos;
    pos += (char)('a' + cord.x);
    pos += (char)('8' - cord.y);
    return pos;
}

std::array<std::array<wchar_t, 6>, 3> pieceAscii{{
    {'♟', '♜', '♞', '♝', '♛', '♚'},
    {'♙', '♖', '♘', '♗', '♕', '♔'},
    {'▢', '▢', '▢', '▢', '▢', '▢'}
}};
