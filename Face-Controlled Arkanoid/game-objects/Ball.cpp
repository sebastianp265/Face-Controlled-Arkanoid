#include "Ball.h"

Ball::Ball(sf::Vector2f position, float radius) : radius(radius)
{
	m_shape.setFillColor(sf::Color::White);
	m_shape.setOrigin(radius, radius);
	m_shape.setRadius(radius);
	m_shape.setPosition(position);
}

void Ball::setPosition(const sf::Vector2f& position)
{
	m_shape.setPosition(position);
}

sf::Vector2f Ball::getPosition()
{
	return m_shape.getPosition();
}

void Ball::setVelocity(const sf::Vector2f& velocity)
{
	this->m_velocity = velocity;
}

sf::Vector2f Ball::getVelocity()
{
	return m_velocity;
}

void Ball::reverseXVelocity()
{
	m_velocity.x *= -1;
}

void Ball::reverseYVelocity()
{
	m_velocity.y *= -1;
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(m_shape, state);
}