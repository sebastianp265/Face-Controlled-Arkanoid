#pragma once
#include <SFML/Graphics.hpp>

class GameObjectsParameters {
public:
	GameObjectsParameters(float windowWidth, float windowHeight) : m_windowWidth(windowWidth), m_windowHeight(windowHeight) {};

	const float m_windowWidth;
	const float m_windowHeight;

	const sf::Vector2f BALL_START_POSITION = { m_windowWidth / 2.0f, m_windowHeight * 0.8f };
	const sf::Vector2f BALL_INITIAL_SPEED = { m_windowWidth / 100.0f, m_windowHeight / 100.0f };
	const float BALL_RADIUS = (m_windowWidth + m_windowHeight) * 0.01f;

	const sf::Vector2f PADDLE_STARTING_POSITION = { m_windowWidth / 2.0f, m_windowHeight * 0.9f };
	const sf::Vector2f PADDLE_SIZE = { m_windowWidth / 5.0f, m_windowHeight / 10.0f };

	const size_t BLOCK_COLUMNS = 5;
	const size_t BLOCK_ROWS = 5;
	// top, left, right margin
	const float BLOCK_FIELD_MARGIN = m_windowWidth / 15.0f;
	const float BLOCK_FIELD_WIDTH = m_windowWidth - 2 * BLOCK_FIELD_MARGIN;
	const float BLOCK_FIELD_HEIGHT = m_windowHeight / 4.0f + BLOCK_FIELD_MARGIN;

	const float BLOCK_MARGIN = m_windowWidth / 65.0f;
	const float BLOCK_WIDTH = (BLOCK_FIELD_WIDTH - (BLOCK_COLUMNS - 1) * BLOCK_MARGIN) / BLOCK_COLUMNS;
	const float BLOCK_HEIGHT = (BLOCK_FIELD_HEIGHT - (BLOCK_ROWS - 1) * BLOCK_MARGIN) / BLOCK_ROWS;
};
