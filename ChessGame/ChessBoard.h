#pragma once
#include "ChessPiece.h"

class ChessBoard {
public:
    ChessBoard();

    void initialize();
    ChessPiece* getPieceAt(int row, int col) const;

private:
    std::vector<std::vector<ChessPiece*>> _board;
};

