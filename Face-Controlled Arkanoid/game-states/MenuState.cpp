#include "MenuState.h"
#include "RunningState.h"

#include <iostream>

MenuState::MenuState(Game& game) : GameState(game), 
m_startGameButton({game.getWindowWidth() / 2.0f, game.getWindowHeight() / 2.0f}, "Start game", m_game.getFont())
{

}

void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		sf::Vector2f mousePosition(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
		m_startGameButton.setHovered(m_startGameButton.isMouseInBounds(mousePosition));
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
		if (m_startGameButton.isMouseInBounds(mousePosition)) {
			m_game.setState(new RunningState(m_game, m_game.getCameraCapture(), m_game.getFaceDetector()));
			delete this;
		}
	}
}

void MenuState::update()
{
}

void MenuState::render()
{
	m_game.getWindow().draw(m_startGameButton);
}