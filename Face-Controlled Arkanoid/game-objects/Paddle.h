#pragma once

#include <SFML/Graphics.hpp>

class Paddle : public sf::Drawable
{
public:
	Paddle(sf::Vector2f position, sf::Vector2f size);
	~Paddle() = default;

	void setPosition(sf::Vector2f position);
	void setXVelocity(float xVelocity);

	sf::Vector2f getVelocity();
	sf::Vector2f getPosition();

	float getRightXBounding() const;
	float getLeftXBounding() const;
	float getUpperYBounding() const;
	float getBottomYBounding() const;

	sf::Vector2f getLeftUpperCornerPos() const;
	sf::Vector2f getRightUpperCornerPos() const;
	sf::Vector2f getLeftBottomCornerPos() const;
	sf::Vector2f getRightBottomCornerPos() const;

	float getWidth() const;
	float getHeight() const;
	
private:
	sf::RectangleShape m_shape;
	sf::Vector2f m_velocity;

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
};