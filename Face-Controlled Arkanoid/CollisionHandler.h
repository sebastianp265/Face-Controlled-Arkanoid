#pragma once
#include "game-objects/Ball.h"
#include "game-objects/Block.h"
#include "game-objects/Paddle.h"

#include <list>
#include <memory>

class CollisionHandler {
public:
	CollisionHandler(Ball& ball, Paddle& paddle, std::list<Block>& blocks, float windowWidth, float windowHeight);
	bool update(float deltaTime);

private:
	const float m_windowWidth;
	const float m_windowHeight;

	enum CollisionType {NONE, BLOCK, BOTTOM_WALL, NOT_BOTTOM_WALL, PADDLE };
	struct CollisionDetails{
		float timeToCollision = std::numeric_limits<float>::infinity();
		sf::Vector2f ballPosAtCollision;
		sf::Vector2f ballVelocityAfterCollision;
		CollisionType collisionType = NONE;
	};

	size_t blockToDestroyIndex = -1;

	Ball& m_ball;
	Paddle& m_paddle;
	std::list<Block>& m_blocks;

	void updatePaddle(float deltaTime);

	bool isInBounds(const float value, const float low, const float high);
	float dotProduct(sf::Vector2f a, sf::Vector2f b);
	float square(float val);

	bool updateEarliestCollisionDetails(CollisionDetails& earliestCollisionDetails,const CollisionDetails& collisionDetails);

	// if collision doesn't occur
	// then returned CollisionDetails object has attribute timeToCollision=std::numeric_limits<float>::infinity();
	CollisionDetails handleBallToWalls();
	CollisionDetails handleBallToPaddle();
	CollisionDetails handleBallToBlocks();

	CollisionDetails handleBallToPoint(const sf::Vector2f& pointPosition);
	CollisionDetails handleBallToHorizontalLine(float lineYPos, float xLowerLimit, float xUpperLimit);
	CollisionDetails handleBallToVerticalLine(float lineXPos, float yLowerLimit, float yUpperLimit);
};