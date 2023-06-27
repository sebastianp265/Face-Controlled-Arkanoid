#include "PauseState.h"
#include "MenuState.h"

PauseState::PauseState(Game& game, GameState* runningState) : GameState(game),
	m_runningState(runningState),
	m_resumeGameButton(sf::Vector2f(m_game.getWindowWidth() * 0.5f, m_game.getWindowHeight() * 0.3f), "Resume", m_game.getFont()),
	m_mainMenuButton(sf::Vector2f(m_game.getWindowWidth() * 0.5f, m_game.getWindowHeight() * 0.4f), "Main Menu", m_game.getFont()),
	m_exitGameButton(sf::Vector2f(m_game.getWindowWidth() * 0.5f, m_game.getWindowHeight() * 0.5f), "Exit Game", m_game.getFont())
{
}

void PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			m_game.setState(std::move(m_runningState));
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		sf::Vector2f mousePosition(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
		m_resumeGameButton.setHovered(m_resumeGameButton.isMouseInBounds(mousePosition));
		m_mainMenuButton.setHovered(m_mainMenuButton.isMouseInBounds(mousePosition));
		m_exitGameButton.setHovered(m_exitGameButton.isMouseInBounds(mousePosition));
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
		if (m_resumeGameButton.isMouseInBounds(mousePosition)) {
			m_game.setState(m_runningState);
			delete this;
		}
		else if (m_mainMenuButton.isMouseInBounds(mousePosition)) {
			delete m_runningState;
			m_game.setState(new MenuState(m_game));
		}
		else if (m_exitGameButton.isMouseInBounds(mousePosition)) {
			m_game.exitGame();
		}
	}
}

void PauseState::update()
{
}

void PauseState::render()
{
	sf::RenderWindow& renderWindow = m_game.getWindow();
	renderWindow.draw(m_resumeGameButton);
	renderWindow.draw(m_mainMenuButton);
	renderWindow.draw(m_exitGameButton);
}

