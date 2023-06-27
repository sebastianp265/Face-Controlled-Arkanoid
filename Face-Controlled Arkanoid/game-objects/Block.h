#pragma once

#include <SFML/Graphics.hpp>

class Block : public sf::Drawable
{
public:
	Block(sf::Vector2f position, float width, float height, sf::Color color, size_t blockNum);
	~Block() = default;

	// BPos - BoundingPosition
	sf::Vector2f getLeftUpperCornerPos() const;
	sf::Vector2f getRightUpperCornerPos() const;
	sf::Vector2f getLeftBottomCornerPos() const;
	sf::Vector2f getRightBottomCornerPos() const;

	float getUpYBouding() const;
	float getBottomYBounding() const;
	float getLeftXBounding() const;
	float getRightXBounding() const;

	const size_t m_blockNum;
private:

	sf::RectangleShape m_shape;

	sf::Vector2f m_leftUpperCornerPos;
	sf::Vector2f m_rightUpperCornerPos;
	sf::Vector2f m_leftBottomCornerPos;
	sf::Vector2f m_rightBottomCornerPos;

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
};