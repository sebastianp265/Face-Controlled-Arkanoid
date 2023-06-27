#pragma once
#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable
{
public:
	const float radius;

	Ball(sf::Vector2f position, float radius);
	~Ball() = default;
	
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition();

	void setVelocity(const sf::Vector2f& velocity);
	sf::Vector2f getVelocity();

	void reverseXVelocity();
	void reverseYVelocity();
private:	
	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
};