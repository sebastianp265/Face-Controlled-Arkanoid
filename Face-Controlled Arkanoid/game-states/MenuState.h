#pragma once
#include "GameState.h"
#include "../UI/Button.h"

class MenuState : public GameState {
public:
	MenuState(Game& game);
	void handleEvent(const sf::Event& event) override;
	void update() override;
	void render() override;

private:
	Button m_startGameButton;
};