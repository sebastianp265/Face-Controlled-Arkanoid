#include "GameResultState.h"
#include "RunningState.h"
#include "MenuState.h"
#include "../UI/TextUtils.h"

GameResultState::GameResultState(Game& game, bool win) : GameState(game),
m_startGameButton(sf::Vector2f( m_game.getWindowWidth() / 2.0f, m_game.getWindowHeight() * 0.5f ), "Start Game", m_game.getFont()),
m_mainMenuButton(sf::Vector2f(m_game.getWindowWidth() / 2.0f, m_game.getWindowHeight() * 0.6f), "Main Menu", m_game.getFont())
{
	m_gameResultText.setFont(m_game.getFont());
	if (win) {
		m_gameResultText.setString("You Won! Congratulations!");
	}
	else {
		m_gameResultText.setString("You Lost! Try Again!");

	}
	m_gameResultText.setOrigin(getLocalSize(m_gameResultText) / 2.0f);
	m_gameResultText.setPosition(m_game.getWindowWidth() / 2.0f, m_game.getWindowHeight() * 0.3f);
}

void GameResultState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		sf::Vector2f mousePosition(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
		m_startGameButton.setHovered(m_startGameButton.isMouseInBounds(mousePosition));
		m_mainMenuButton.setHovered(m_mainMenuButton.isMouseInBounds(mousePosition));
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
		if (m_startGameButton.isMouseInBounds(mousePosition)) {
			m_game.setState(new RunningState(m_game, m_game.getCameraCapture(), m_game.getFaceDetector()));
			delete this;
		}
		else if (m_mainMenuButton.isMouseInBounds(mousePosition)) {
			m_game.setState(new MenuState(m_game));
			delete this;
		}
	}
}

void GameResultState::update()
{
}

void GameResultState::render()
{
	sf::RenderWindow& renderWindow = m_game.getWindow();
	renderWindow.draw(m_gameResultText);
	renderWindow.draw(m_startGameButton);
	renderWindow.draw(m_mainMenuButton);
}
