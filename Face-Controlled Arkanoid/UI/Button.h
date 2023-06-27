#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable {
public:
	Button(sf::Vector2f position, std::string buttonText, const sf::Font& font);
	Button() = delete;
	bool isMouseInBounds(sf::Vector2f mousePosition);
	void setHovered(bool hovered);
private:
	sf::Text buttonText;
	const sf::Vector2f position;
	sf::Vector2f size;

	const sf::Color basicColor{224, 224, 224};
	const sf::Color hoveredColor{160, 160, 160};

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
};