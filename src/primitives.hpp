#pragma once
#include <bits/stdc++.h>
#include "literals.hpp"

struct Cordinate {
    uint8_t x : 4;
    uint8_t y : 4;

    Cordinate();
    Cordinate(uint8_t, uint8_t);
    //Cordinate(nCordinate);

    Cordinate operator+(Cordinate&) const;
};

Cordinate::Cordinate() {}

Cordinate::Cordinate(uint8_t x, uint8_t y) : x(x), y(y) {}

//Cordinate::Cordinate(nCordinate nc) : x(static_cast<uint8_t>(nc.x)), y(static_cast<uint8_t>(nc.y)) {}

Cordinate Cordinate::operator+(Cordinate& other) const {
    return Cordinate(x + other.x, y + other.y);
}

struct nCordinate {
    int8_t x;
    int8_t y;

    nCordinate();
    nCordinate(int8_t, int8_t);

    operator Cordinate() const;
};

nCordinate::nCordinate() {}

nCordinate::nCordinate(int8_t x, int8_t y) : x(x), y(y) {}

nCordinate::operator Cordinate() const {
    return Cordinate{x, y};
}

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

struct Difference {
    int8_t xDiff;
    int8_t yDiff;
};

nCordinate operator+(Cordinate c, Difference d) {
    return nCordinate{static_cast<int8_t>(c.x) + d.xDiff, static_cast<int8_t>(c.y) + d.yDiff};
}

bool inBounds(Cordinate cord) { return (cord.x < 8) && (cord.y < 8); }

uint8_t opponent(uint8_t player) { return (player + 1) % 2; }

bool inBounds(nCordinate nCord) {
    return 
        (nCord.x < 8) && 
        (-1 < nCord.x) &&
        (nCord.y < 8) &&
        (-1 < nCord.y);
}
