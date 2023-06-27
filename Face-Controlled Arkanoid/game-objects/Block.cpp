#include "Block.h"

Block::Block(sf::Vector2f position, float width, float height, sf::Color color, size_t blockNum)
	: m_blockNum(blockNum)
{
	m_shape.setFillColor(color);
	m_shape.setSize({width, height});
	m_shape.setOrigin({ width / 2.0f, height / 2.0f });
	m_shape.setPosition(position);

	m_rightBottomCornerPos = { position.x + width / 2.0f, position.y + height / 2.0f };
	m_rightUpperCornerPos = { position.x + width / 2.0f, position.y - height / 2.0f };
	m_leftBottomCornerPos = { position.x - width / 2.0f, position.y + height / 2.0f };
	m_leftUpperCornerPos = { position.x - width / 2.0f, position.y - height / 2.0f };
}

sf::Vector2f Block::getLeftUpperCornerPos() const
{
	return m_leftUpperCornerPos;
}

sf::Vector2f Block::getRightUpperCornerPos() const
{
	return m_rightUpperCornerPos;
}

sf::Vector2f Block::getLeftBottomCornerPos() const
{
	return m_leftBottomCornerPos;
}

sf::Vector2f Block::getRightBottomCornerPos() const
{
	return m_rightBottomCornerPos;
}

float Block::getUpYBouding() const
{
	return m_rightUpperCornerPos.y;
}

float Block::getBottomYBounding() const
{
	return m_rightBottomCornerPos.y;
}

float Block::getLeftXBounding() const
{
	return m_leftBottomCornerPos.x;
}

float Block::getRightXBounding() const
{
	return m_rightBottomCornerPos.x;
}

void Block::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(m_shape, state);
}
