#include "Button.h"
#include "TextUtils.h"

Button::Button(sf::Vector2f position, std::string buttonContent, const sf::Font& font) :
	buttonText(buttonContent, font),
	position(position),
	size(getLocalSize(buttonText))
{
	buttonText.setFillColor(basicColor);
	buttonText.setOrigin(size / 2.0f);
	buttonText.setPosition(position);
}

bool Button::isMouseInBounds(sf::Vector2f mousePosition)
{
	if (mousePosition.x >= position.x - size.x / 2.0f
		&& mousePosition.x <= position.x + size.x / 2.0f
		&& mousePosition.y >= position.y - size.y / 2.0f
		&& mousePosition.y <= position.y + size.y / 2.0f)
	{
		return true;
	}
	return false;
}

void Button::setHovered(bool hovered)
{
	if (hovered) {
		buttonText.setFillColor(hoveredColor);
	}
	else {
		buttonText.setFillColor(basicColor);
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(buttonText);
}
