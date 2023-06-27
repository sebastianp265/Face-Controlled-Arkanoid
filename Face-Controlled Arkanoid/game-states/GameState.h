#pragma once
#include "SFML/Graphics.hpp"
#include "../Game.h"
class Game;

class GameState {
public:
	explicit GameState(Game& game) : m_game(game) {};
	virtual ~GameState() = default;

	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void update() = 0;
	virtual void render() = 0;
protected:
	Game& m_game;
};