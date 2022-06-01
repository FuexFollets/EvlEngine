#include "primitives.hpp"
#include "difference_literals.hpp"
#pragma once

class ChessGame {
    ChessGrid grid;

    uint8_t lastDoublePawnMove{8}; // used for en passant
    // 0 - 7 used for rows, any larger means last move was not a double pawn move
    
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

    MoveList squareMoves(const Cordinate cord);

    MoveList avalibleMoves();

    MoveList legalMoves();
};

void ChessGame::makeMove(const ChessMove move) {
    switch(move.moveType) {
        case 0: // Normal move
            atCord(move.end) = atCord(move.start);
            atCord(move.start) = Piece();
            break;
        case 1: // Castle
            if (move.castleType == 0) {
                atCord(2, (1 - move.color) * 7) = Piece(move.color, piece_literals.king);
                atCord(3, (1 - move.color) * 7) = Piece(move.color, piece_literals.rook);
            } else {
                atCord(6, (1 - move.color) * 7) = Piece(move.color, piece_literals.king);
                atCord(5, (1 - move.color) * 7) = Piece(move.color, piece_literals.rook);
            }
        case 2: // En passant
            atCord(move.pawnRow, move.color + 3) = Piece();
            atCord(move.pawnRow, 3 * move.color + 2) = Piece(move.color, 0);
            break;

    }
}

MoveList ChessGame::squareMoves(const Cordinate cord) { // moves for a singular square
    MoveList av_moves;
    Piece piece{atCord(cord)};

    if (piece.color == 2) return av_moves;

    switch(static_cast<int>(piece.type)) {
        case 0: // Pawn
            int pawnDirection{(1 - piece.color) * 2};
            if (atCord(cord.x + pawnDirection, cord.y).color != 2) break;
            av_moves.push_back(ChessMove(cord, Cordinate(cord.x + pawnDirection, cord.y)));

            if (atCord(cord.x + 2 * pawnDirection, cord.y).color != 2) break;
            av_moves.push_back(ChessMove(cord, Cordinate(cord.x + 2 * pawnDirection, cord.y)));

            if (lastDoublePawnMove > 8) break;

            if (lastDoublePawnMove != cord.y) break;

            int take_side{static_cast<int>(lastDoublePawnMove) - static_cast<int>(cord.x)};
            if (std::abs(take_side) != 1) break;

            av_moves.push_back(ChessMove(cord.x, static_cast<uint8_t>((take_side + 1) / 2), piece.color));

            break;

        case 2: // Knight
            for (const auto& [xDifference, yDifference]: diff::knight) {
                if (!inBounds(cord + Cordinate(xDifference, yDifference))) continue;
                if (atCord(cord + Cordinate(xDifference, yDifference)).color == piece.color) continue;
                av_moves.push_back(ChessMove(cord, cord + Cordinate(xDifference, yDifference)));
            }
            break;

        case 5: // King
            for (const auto& [xDifference, yDifference]: diff::king) {
                if (inBounds(cord + Cordinate(xDifference, yDifference))) continue;
                if (atCord(cord + Cordinate(xDifference, yDifference)).color == piece.color) continue;
                av_moves.push_back(ChessMove(cord, cord + Cordinate(xDifference, yDifference)));
            }
            break;

        case 4: // Queen
        case 3: // Bishop
            for (const auto& direction: diff::rook) for (const auto&[xDifference, yDifference]: direction) {
                if (!inBounds(cord + Cordinate(xDifference, yDifference))) break;
                if (atCord(cord + Cordinate(xDifference, yDifference)).color == piece.color) break;
                av_moves.push_back(ChessMove(cord, cord + Cordinate(xDifference, yDifference)));
                if (atCord(cord + Cordinate(xDifference, yDifference)).color != piece.color) break; 
            }

            if (piece.type != piece_literals.queen) break;   
        case 1: // Rook
            for (const auto& direction: diff::rook) for (const auto&[xDifference, yDifference]: direction) {
                if (!inBounds(cord + Cordinate(xDifference, yDifference))) break;
                if (atCord(cord + Cordinate(xDifference, yDifference)).color == piece.color) break;
                av_moves.push_back(ChessMove(cord, cord + Cordinate(xDifference, yDifference)));
                if (atCord(cord + Cordinate(xDifference, yDifference)).color != piece.color) break; 
            }
            break;
    }

    return av_moves;
}
