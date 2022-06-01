#pragma once
#include <bits/stdc++.h>
#include "piece_literals.hpp"

struct Cordinate {
    uint8_t x : 4;
    uint8_t y : 4;

    Cordinate(uint8_t x, uint8_t y) : x(x), y(y) {}
    Cordinate() : x(0), y(0) {}

    Cordinate operator+(Cordinate& other) const {
        return Cordinate(x + other.x, y + other.y);
    }
};

struct nCordinate {
    int16_t x : 8;
    int16_t y : 8;

    nCordinate(int16_t x, int16_t y) : x(x), y(y) {}
    nCordinate() : x(0), y(0) {}

    operator Cordinate() const {
        return Cordinate{static_cast<uint8_t>(x), static_cast<uint8_t>(y)};
    } 
};

// 0 = White, 1 = Black
// 0 = Pawn, 1 = Rook, 2 = Knight, 3 = Bishop, 4 = Queen, 5 = King
// Piece{2, x} = Blank
struct Piece {
    uint8_t type : 4;
    uint8_t color : 4;
    Piece(uint8_t color, uint8_t type) : type(type), color(color) {}
    Piece() : color(color_literals.blank) {}
};

// Evil bitfield compression
struct ChessMove { // 0 = normal, 1 = castling, 2 = en passant // 32 bytes
    uint8_t moveType : 2; // 2 bits

    uint8_t color : 1; // 1 bit

    // Normal move
    Cordinate start; // 8 bits
    Cordinate end; // 8 bits

    // Castling
    uint8_t rookStart : 3; // 3 bits
    uint8_t kingStart : 3; // 3 bits
    uint8_t castleType : 1; // 1 bit // 0 = queenside, 1 = kingside

    // En passant
    uint8_t pawnRow : 3; // 3 bits
    uint8_t enSide : 1; // 1 bit // 0 = left, 1 = right

    ChessMove(Cordinate from, Cordinate to) : moveType(0), start(from), end(to) {} // Normal move
    ChessMove(uint8_t color, uint8_t side, uint8_t rookStart, uint8_t kingStart) : moveType(1), rookStart(rookStart), kingStart(kingStart), color(color), castleType(side) {} // Castling
    ChessMove(uint8_t row, uint8_t side, uint8_t color) : moveType(2), pawnRow(row), enSide(side), color(color) {} // En passant
};

typedef std::array<std::array<Piece, 8>, 8> ChessGrid;

typedef std::vector<ChessMove> MoveList;

struct Difference {
    int8_t xDiff;
    int8_t yDiff;
};

nCordinate operator+(Cordinate c, Difference d) {
    return nCordinate{static_cast<int16_t>(c.x) + d.xDiff, static_cast<int16_t>(c.y) + d.yDiff};
}

bool inBounds(nCordinate nCord) {
    return 
        (nCord.x < 8) && 
        (-1 < nCord.x) &&
        (nCord.y < 8) &&
        (-1 < nCord.y);
}

bool inBounds(Cordinate cord) {
    return (cord.x < 8) && (cord.y < 8);
}
