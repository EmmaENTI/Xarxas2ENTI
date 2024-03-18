#include "ChessBoard.h"

ChessBoard::ChessBoard() : _board(8, std::vector<ChessPiece*>(8, nullptr)) {}

void ChessBoard::initialize() {
    // Lógica para inicializar el tablero de ajedrez con las piezas en sus posiciones iniciales
    // Crea e inicializa las piezas y colócalas en la matriz _board
}

ChessPiece* ChessBoard::getPieceAt(int row, int col) const {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return _board[row][col];
    }
    return nullptr;
}