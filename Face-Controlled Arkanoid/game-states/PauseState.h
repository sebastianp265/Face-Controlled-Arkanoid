#pragma once
#include "GameState.h"
#include "RunningState.h"
#include "../UI/Button.h"

class PauseState : public GameState {
public:
	PauseState(Game& game, GameState* runningState); // used for switching between pause to running without reseting whole game
	void handleEvent(const sf::Event& event) override;
	void update() override;
	void render() override;
private:
	GameState* m_runningState;
	Button m_resumeGameButton;
	Button m_mainMenuButton;
	Button m_exitGameButton;
};