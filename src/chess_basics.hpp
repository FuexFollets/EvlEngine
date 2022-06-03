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
        case movetype_literals.normal: // Normal move
            atCord(move.endX, move.endY) = atCord(move.startX, move.startY);
            atCord(move.startX, move.startY).color = color_literals.blank;
            break;

        case movetype_literals.castle: // Castling
            if (move.castleType == 0) {
                atCord(6, 7 - move.color * 7) = Piece(piece_literals.king, move.color);
                atCord(5, 7 - move.color * 7) = Piece(piece_literals.rook, move.color);
                atCord(7, 7 - move.color * 7).color = color_literals.blank;
                atCord(7, 7 - move.color * 7).color = color_literals.blank;
            } else {
                atCord(2, 7 - move.color * 7) = Piece(piece_literals.king, move.color);
                atCord(3, 7 - move.color * 7) = Piece(piece_literals.rook, move.color);
                atCord(0, 7 - move.color * 7).color = color_literals.blank;
                atCord(0, 7 - move.color * 7).color = color_literals.blank;
            }
            break;

        case movetype_literals.en_passant:
            atCord(move.endX, move.endY) = Piece(piece_literals.pawn, move.color);
            atCord(move.startX, move.startY).color = color_literals.blank;
            atCord(move.endX, move.startY).color = color_literals.blank;
            break;

        case movetype_literals.promotion:
            atCord(move.endX, move.endY) = Piece(move.promotionType, move.color);
            atCord(move.startX, move.startY).color = color_literals.blank;
            break;
    }
}

MoveList ChessGame::squareMoves(const Cordinate cord) { // moves for a singular square
    MoveList av_moves;
    Piece piece{atCord(cord)};

    if (piece.color == 2) return av_moves;

    switch(piece.type) {
        case piece_literals.pawn: {
            if (cord.y == 1 + piece.color * 5) { // pawn promotion
                for (uint8_t piece_type{1}; piece_type < 5; piece_type++) {
                    av_moves.push_back(ChessMove(piece.color, Piece(piece_type, piece.color)));
                }
                break;
            }

            if (atCord(cord.x, cord.y + piece.color * 2 - 1).color != color_literals.blank) { // pawn capture normal
                for (const Difference& side: difference_literals.LeftRight) {
                    if (!inBounds(cord + side)) continue;
                    if (atCord(cord + side).color != opponent(piece.color)) continue;
                    av_moves.push_back(ChessMove(cord, Cordinate(cord.x + side.x, cord.y + piece.color * 2 - 1)));
                }
                break;
            }
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
