#include "primitives.hpp"

namespace diff { // used for avaible move calculation
    const std::array<Difference, 8> king{[]{
        std::array<Difference, 8> king;
        int pos{0};
        for (int8_t x{-1}; x <= 1; x++) for (int8_t y{-1}; y <= 1; y++) {
                if (x == 0 && y == 0) continue;
                king[pos++] = Difference{x, y};
        }

        return king;
    }()};
    
    const std::array<Difference, 8> knight{
    {
        Difference{-2, -1},
        Difference{-2, 1},
        Difference{-1, -2},
        Difference{-1, 2},
        Difference{1, -2},
        Difference{1, 2},
        Difference{2, -1},
        Difference{2, 1}
    }};

    const std::array<std::array<Difference, 7>, 4> bishop{[]{
        std::array<std::array<Difference, 7>, 4> bishop;
        int8_t direction{};
        for (int8_t xAX{-1}; xAX < 2; xAX += 2) for (int8_t yAX{-1}; yAX < 2; yAX += 2) {
            for (int8_t distance{1}; distance < 8; distance++) {
                bishop[direction][distance - 1] = Difference{xAX * distance, yAX * distance};
            }
        };

        return bishop;
    }()
    };

    std::array<std::array<Difference, 7>, 4> rook;

    std::array<std::array<Difference, 7>, 8> queen; 

}