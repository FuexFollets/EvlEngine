#pragma once
#include <stdint.h>

constexpr struct {
    uint8_t pawn{0};
    uint8_t knight{1};
    uint8_t bishop{2};
    uint8_t rook{3};
    uint8_t queen{4};
    uint8_t king{5};
} piece_literals;

constexpr struct {
    uint8_t white{0};
    uint8_t black{1};
    uint8_t blank{2};
} color_literals;

