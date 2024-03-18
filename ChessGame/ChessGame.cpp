#include "ChessGame.h"

ChessGame::ChessGame() {}

void ChessGame::init() 
{
    _board.initialize();
}

ChessBoard& ChessGame::getBoard() 
{
    return _board;
}
