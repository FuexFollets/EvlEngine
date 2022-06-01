#include "primitives.hpp"
#include "difference_literals.hpp"
#include "piece_literals.hpp"
#pragma once

class ChessGame {
    uint8_t turn : 1;
    ChessGrid grid;
    uint8_t lastDoublePawnMove{8}; // used for en passant
    // 0 - 7 used for rows, any larger means last move was not a double pawn move
    
    struct {
        bool wk : 1;
        bool wq : 1;
        bool bk : 1;
        bool bq : 1;
    } castleRights;

    Piece& atCord(const Cordinate c) { return grid[c.y][c.x]; }
    Piece& atCord(const nCordinate nc) { return grid[nc.y][nc.x]; }
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

    switch(piece.type) {
        case piece_literals.pawn: {
            auto pawnDirection{(1 - piece.color) * 2};
            if (atCord(cord.x + pawnDirection, cord.y).color != 2) break;
            
            av_moves.push_back(ChessMove(cord, Cordinate(cord.x + pawnDirection, cord.y)));

            if (atCord(cord.x + 2 * pawnDirection, cord.y).color != 2) break;
            
            av_moves.push_back(ChessMove(cord, Cordinate(cord.x + 2 * pawnDirection, cord.y)));

            if (lastDoublePawnMove > 8) break;
            
            if (lastDoublePawnMove != cord.y) break;

            int take_side{static_cast<int>(lastDoublePawnMove) - static_cast<int>(cord.x)};
            
            if (std::abs(take_side) != 1) break;

            av_moves.push_back(ChessMove(cord.x, static_cast<uint8_t>((take_side + 1) / 2), piece.color));
        }
            break;
        
        case piece_literals.knight: {
            for (const auto& cordDifference: diff::knight) {
                if (!inBounds(cord + cordDifference)) continue;
                if (atCord(cord + cordDifference).color == piece.color) continue;
                av_moves.push_back(ChessMove(cord, cord + cordDifference));
            }
        }
            break;

        case piece_literals.queen:
        case piece_literals.bishop: {
            for (const auto& direction: diff::rook) for (const auto& cordDifference: direction) {
                if (!inBounds(cord + cordDifference)) break;
                if (atCord(cord + cordDifference).color == piece.color) break;
                av_moves.push_back(ChessMove(cord, cord + cordDifference));
                if (atCord(cord + cordDifference).color != piece.color) break; 
            }
        }
            if (piece.type != piece_literals.queen) break;
    
        case piece_literals.rook: {
            for (const auto& direction: diff::rook) for (const auto& cordDifference: direction) {
                if (!inBounds(cord + cordDifference)) break;
                if (atCord(cord + cordDifference).color == piece.color) break;
                av_moves.push_back(ChessMove(cord, cord + cordDifference));
                if (atCord(cord + cordDifference).color != piece.color) break; 
            }
        }
            break;

        case piece_literals.king: {
            for (const auto& cordDifference: diff::king) {
                if (inBounds(cord + cordDifference)) continue;
                if (atCord(cord + cordDifference).color == piece.color) continue;
                av_moves.push_back(ChessMove(cord, cord + cordDifference));
            }
            break;
        }
    }

    return av_moves;
}
