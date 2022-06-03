#pragma once
#include "primitives.hpp"
#include "difference_literals.hpp"
#include "literals.hpp"

typedef std::array<std::array<Piece, 8>, 8> ChessGrid;

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

    ChessMove();
    ChessMove(uint8_t, Cordinate, Cordinate); // Normal move
    ChessMove(uint8_t, uint8_t, uint8_t, uint8_t); // Castling
    ChessMove(uint8_t, uint8_t, uint8_t); // En passant // 0 = left, 1 = right
    ChessMove(uint8_t, Piece); // Promotion
};

ChessMove::ChessMove() {}

ChessMove::ChessMove(uint8_t color, Cordinate start, Cordinate end) : 
        moveType(movetype_literals.normal), color(color), startX(start.x), startY(start.y), endX(end.x), endY(end.y), castleType(0), piece_type(0) {}

ChessMove::ChessMove(uint8_t color, uint8_t castleType, uint8_t rookStart, uint8_t kingstart) : 
        moveType(movetype_literals.castle), color(color), startX(rookStart), startY(kingstart), endX(0), endY(0), castleType(castleType), piece_type(0) {}

ChessMove::ChessMove(uint8_t color, uint8_t row, uint8_t take_side) : 
        moveType(movetype_literals.en_passant), color(color), startX(row), startY(3 + color), endX(row - take_side * 2), endY(2 + 3 * color), castleType(0), piece_type(0) {}

ChessMove::ChessMove(uint8_t row, Piece piece) : 
        moveType(movetype_literals.promotion), color(piece.color), startX(row), startY(1 + 5 * color), endX(row), endY(7 * color), castleType(0), piece_type(piece.type) {}

typedef std::vector<ChessMove> MoveList;

class ChessGame {
    uint8_t turn : 1;
    ChessGrid grid;

    struct {
        bool wk : 1;
        bool wq : 1;
        bool bk : 1;
        bool bq : 1;
    } castleRights;
    
    struct {
        uint8_t row : 3;
        uint8_t side : 1;
        bool relevant : 1;
    } enp; // En passant

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

    std::string exportFEN(); // Conversion to FEN

    ChessGame(std::string fen); // Construcion from FEN
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
            atCord(move.endX, move.endY) = Piece(move.piece_type, move.color);
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
                for (const Difference& side: diff::PawnCap[piece.color]) {
                    if (!inBounds(cord + side)) continue;
                    if (atCord(cord + side).color != opponent(piece.color)) continue;
                    av_moves.push_back(ChessMove(cord, Cordinate(cord.x + side.xDiff, cord.y + side.yDiff)));
                }
                break;
            }
        }
            break;
        
        case piece_literals.knight: {
            for (const auto& cordDifference: diff::knight) {
                if (!inBounds(cord + cordDifference)) continue;
                if (atCord(cord + cordDifference).color == piece.color) continue;
                av_moves.push_back(ChessMove(cord, Cordinate(cord + cordDifference)));
            }
        }
            break;

        case piece_literals.queen:
        case piece_literals.bishop: {
            for (const auto& direction: diff::rook) for (const auto& cordDifference: direction) {
                if (!inBounds(cord + cordDifference)) break;
                if (atCord(cord + cordDifference).color == piece.color) break;
                av_moves.push_back(ChessMove(cord, Cordinate(cord + cordDifference)));
                if (atCord(cord + cordDifference).color != piece.color) break; 
            }
        }
            if (piece.type != piece_literals.queen) break;
    
        case piece_literals.rook: {
            for (const auto& direction: diff::rook) for (const auto& cordDifference: direction) {
                if (!inBounds(cord + cordDifference)) break;
                if (atCord(cord + cordDifference).color == piece.color) break;
                av_moves.push_back(ChessMove(cord, Cordinate(cord + cordDifference)));
                if (atCord(cord + cordDifference).color != piece.color) break; 
            }
        }
            break;

        case piece_literals.king: {
            for (const auto& cordDifference: diff::king) {
                if (inBounds(cord + cordDifference)) continue;
                if (atCord(cord + cordDifference).color == piece.color) continue;
                av_moves.push_back(ChessMove(cord, Cordinate(cord + cordDifference)));
            }
            break;
        }
    }

    return av_moves;
}

std::string ChessGame::exportFEN() {
    std::string fen;

    int row_number{};
    int blank_spaces{};
    int col_number{};
    
    for (const auto& row : grid) {
        for (const Piece& piece : row) {
            if ((blank_spaces != 0) &&
                (col_number == 7)) {
                fen += (char)(blank_spaces + '0');
                continue;
            }
            
            if ((blank_spaces != 0) &&
                (piece.color != 2)) {
                fen += (char)(blank_spaces + '0');
                fen += piece_letter[piece];
                continue;
            }

            if (piece.color != 2) {
                fen += piece_letter[piece];
                continue;
            }
            
            ++blank_spaces;
            
        }
        col_number = 0;
        if (++row_number == 7) break;
        fen += '/';
    } // Board values

    fen += (turn == 0) ? (" w") : (" b"); // Turn

    constexpr std::array<char, 4> c_rights{{'K', 'Q', 'k', 'q'}};
    int right_number{};
    for (const auto& right : std::array<bool, 4>{castleRights.wk, castleRights.wq, bk, bq}) {
        fen += (right) : castle_rights[right_number] ? "";
        ++right_number;
    }

    if (!enp.relevant) { fen += " -"; }
    else {
        
    }
}
