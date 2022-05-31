#include "primitives.hpp"

class ChessGame {
    ChessGrid grid;

    uint8_t lastDoublePawnMove : 3;
    
    struct {
        bool wk : 1;
        bool wq : 1;
        bool bk : 1;
        bool bq : 1;
    } castleRights;

    uint8_t turn : 1;

    Piece& atCord(const Cordinate c) { return grid[c.y][c.x]; }
    Piece& atCord(uint8_t x, uint8_t y) { return grid[y][x]; }

    public:

    uint8_t getTurn() { return turn; }
    ChessGrid getGrid() { return grid; }


    void makeMove(const ChessMove);

    std::vector<ChessMove> avalibleMoves();

    std::vector<ChessMove> legalMoves();
};

void ChessGame::makeMove(const ChessMove move) {
    switch(move.moveType) {
        case 0: // Normal move
            atCord(move.end) = atCord(move.start);
            atCord(move.start) = Piece();
            break;
        case 1: // Castle
            if (move.castleType == 0) {
                atCord(2, (1 - move.color) * 7) = Piece(move.color, 5);
                atCord(3, (1 - move.color) * 7) = Piece(move.color, 1);
            } else {
                atCord(6, (1 - move.color) * 7) = Piece(move.color, 5);
                atCord(5, (1 - move.color) * 7) = Piece(move.color, 1);
            }
        case 2: // En passant
            atCord(move.pawnRow, move.color + 3) = Piece();
            atCord(move.pawnRow, 3 * move.color + 2) = Piece(move.color, 0);
            break;

    }
}