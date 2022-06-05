#pragma once
#include <stdint.h>
#include <string>

constexpr struct {
    uint8_t pawn{0};
    uint8_t knight{1};
    uint8_t bishop{2};
    uint8_t rook{3};
    uint8_t queen{4};
    uint8_t king{5};
    uint8_t blank{0};
} piece_literals;

constexpr struct {
    uint8_t white{0};
    uint8_t black{1};
    uint8_t blank{2};
} color_literals;

constexpr struct {
    uint8_t normal{0};
    uint8_t castle{1};
    uint8_t en_passant{2};
    uint8_t promotion{3};
} movetype_literals;

const std::string defualt_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
