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

    bool operator==(const Piece& other) const {
        return type == other.type && color == other.color;
    }
};

// Evil bitfield compression
struct ChessMove { // 0 = normal, 1 = castling, 2 = en passant, 3 = Promotion // 32 bytes
    uint8_t moveType : 2; // 2 bits

    uint8_t color : 1; // 1 bit

    uint8_t startX : 3; // 3 bits
    uint8_t startY : 3; // 3 bits

    uint8_t endX : 3; // 3 bits
    uint8_t endY : 3; // 3 bits

    uint8_t castleType : 2; // 2 bits

    uint8_t piece_type : 3; // 3 bits

    ChessMove() {}
    ChessMove(uint8_t color, Cordinate start, Cordinate end) : 
        moveType(movetype_literals.normal), color(color), startX(start.x), startY(start.y), endX(end.x), endY(end.y), castleType(0), piece_type(0) {} // Normal move
    ChessMove(uint8_t color, uint8_t castleType, uint8_t rookStart, uint8_t kingstart) : 
        moveType(movetype_literals.castle), color(color), startX(rookStart), startY(kingstart), endX(0), endY(0), castleType(castleType), piece_type(0), {} // Castling
    ChessMove(uint8_t color, uint8_t row, uint8_t take_side) : 
        moveType(movetype_literals.en_passant), color(color), startX(row), startY(3 + color), endX(row - take_side * 2), endY(2 + 3 * color), castleType(0), piece_type(0) {} // En passant // 0 = left, 1 = right
    ChessMove(uint8_t row, Piece piece) : 
        moveType(movetype_literals.promotion), color(piece.color), startX(row), startY(1 + 5 * color), endX(row), endY(7 * color), castleType(0), piece_type(piece.type) {} // Promotion
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

uint8_t opponent(uint8_t color) {
    return (color + 1) % 2;
}

/*
namespace std {
    template <> struct hash <Piece> {
        size_t operator()(const Piece& piece) const {
            return hash<uint8_t>(*reinterpret_cast<uint8_t*>(&piece));
        }
    };
}
*/