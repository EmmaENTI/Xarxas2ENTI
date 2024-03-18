#include "ChessPiece.h"

ChessPiece::ChessPiece(PieceType type, bool isWhite) : _type(type), _isWhite(isWhite) {}

void ChessPiece::setPosition(sf::Vector2f position) {
    _sprite.setPosition(position);
}

sf::Vector2f ChessPiece::getPosition() const {
    return _sprite.getPosition();
}

void ChessPiece::setTexture(const sf::Texture& texture) {
    _sprite.setTexture(texture);
}
