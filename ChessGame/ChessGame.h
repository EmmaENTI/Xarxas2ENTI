#pragma once

#include "ChessBoard.h"

class ChessGame {
public:
    ChessGame();

    void init();
    ChessBoard& getBoard();

private:
    ChessBoard _board;
};
