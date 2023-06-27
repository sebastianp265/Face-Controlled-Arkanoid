#include "Paddle.h"

Paddle::Paddle(sf::Vector2f position, sf::Vector2f size)
{
	m_shape.setFillColor(sf::Color::Red);
	m_shape.setSize(size);
	m_shape.setOrigin(size / 2.0f);

	m_shape.setPosition(position.x, position.y);
}

//void Paddle::moveRight(float deltaTime)
//{
//	this->shape.move(xVelocity * deltaTime, 0);
//}
//
//void Paddle::moveLeft(float deltaTime) {
//	this->shape.move(-xVelocity * deltaTime, 0);
//}

void Paddle::setPosition(sf::Vector2f position)
{
	m_shape.setPosition(position);
}

void Paddle::setXVelocity(float xVelocity)
{
	m_velocity.x = xVelocity;
}

sf::Vector2f Paddle::getVelocity()
{
	return m_velocity;
}

sf::Vector2f Paddle::getPosition()
{
	return m_shape.getPosition();
}

float Paddle::getWidth() const
{
	return m_shape.getSize().x;
}

float Paddle::getHeight() const
{
	return m_shape.getSize().y;
}

sf::Vector2f Paddle::getLeftUpperCornerPos() const
{
	sf::Vector2f res;
	res.x = m_shape.getPosition().x - m_shape.getSize().x / 2.0f;
	res.y = m_shape.getPosition().y - m_shape.getSize().y / 2.0f;
	return res;
}

sf::Vector2f Paddle::getRightUpperCornerPos() const
{
	sf::Vector2f res;
	res.x = m_shape.getPosition().x + m_shape.getSize().x / 2.0f;
	res.y = m_shape.getPosition().y - m_shape.getSize().y / 2.0f;
	return res;
}

sf::Vector2f Paddle::getLeftBottomCornerPos() const
{
	sf::Vector2f res;
	res.x = m_shape.getPosition().x - m_shape.getSize().x / 2.0f;
	res.y = m_shape.getPosition().y + m_shape.getSize().y / 2.0f;
	return res;
}

sf::Vector2f Paddle::getRightBottomCornerPos() const
{
	sf::Vector2f res;
	res.x = m_shape.getPosition().x + m_shape.getSize().x / 2.0f;
	res.y = m_shape.getPosition().y + m_shape.getSize().y / 2.0f;
	return res;
}

float Paddle::getUpperYBounding() const
{
	return m_shape.getPosition().y - m_shape.getSize().y / 2.0f;
}

float Paddle::getBottomYBounding() const
{
	return m_shape.getPosition().y + m_shape.getSize().y / 2.0f;
}

float Paddle::getLeftXBounding() const
{
	return m_shape.getPosition().x - m_shape.getSize().x / 2.0f;
}

float Paddle::getRightXBounding() const
{
	return m_shape.getPosition().x + m_shape.getSize().x / 2.0f;
}

void Paddle::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(m_shape, state);
}