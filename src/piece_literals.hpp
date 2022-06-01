#pragma once
#include <stdint.h>

struct pl {
    uint8_t pawn{0};
    uint8_t rook{1};
    uint8_t knight{2};
    uint8_t bishop{3};
    uint8_t queen{4};
    uint8_t king{5};
} piece_literals;

