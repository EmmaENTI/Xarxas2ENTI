#pragma once
#include <SFML/Graphics.hpp>

class ChessPiece {
public:
    enum class PieceType { Pawn, Knight, Bishop, Rook, Queen, King };

    ChessPiece(PieceType type, bool isWhite);

    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition() const;

    void setTexture(const sf::Texture& texture);

private:
    PieceType _type;
    bool _isWhite;
    sf::Sprite _sprite;
};