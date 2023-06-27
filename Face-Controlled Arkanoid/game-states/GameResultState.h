#pragma once
#include "GameState.h"
#include "../UI/Button.h"

class GameResultState : public GameState {
public:
	GameResultState(Game& game, bool win);
	void handleEvent(const sf::Event& event) override;
	void update() override;
	void render() override;

private:
	sf::Text m_gameResultText;
	Button m_startGameButton;
	Button m_mainMenuButton;
};