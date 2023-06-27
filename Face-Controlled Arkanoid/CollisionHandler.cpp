#include "CollisionHandler.h"
#include <cassert>

CollisionHandler::CollisionHandler(Ball& ball, Paddle& paddle, std::list<Block>& blocks, float windowWidth, float windowHeight) :
	m_ball(ball), 
	m_paddle(paddle), 
	m_blocks(blocks),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight)
{
}

bool CollisionHandler::update(float deltaTime)
{
	float timeLeft = deltaTime;
	updatePaddle(deltaTime);

	CollisionDetails earliestCollisionDetails;

	while (true) {
		earliestCollisionDetails.timeToCollision = std::numeric_limits<float>::infinity();
		
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToWalls());
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToBlocks());
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToPaddle());

		// if collision hasn't occured - ball moves normally for the timeLeft
		if (earliestCollisionDetails.timeToCollision == std::numeric_limits<float>::infinity()
			|| timeLeft < earliestCollisionDetails.timeToCollision) {
			m_ball.setPosition(m_ball.getPosition() + m_ball.getVelocity() * timeLeft );
			break;
		}

		if (earliestCollisionDetails.collisionType == BOTTOM_WALL) {
			return true;
		}

		if (earliestCollisionDetails.collisionType == BLOCK) {
			m_blocks.remove_if([=](Block block) { return block.m_blockNum == blockToDestroyIndex; });
		}

		m_ball.setPosition(earliestCollisionDetails.ballPosAtCollision);
		m_ball.setVelocity(earliestCollisionDetails.ballVelocityAfterCollision);

		timeLeft -= earliestCollisionDetails.timeToCollision;
	}
	return false;
}

bool CollisionHandler::isInBounds(const float value, const float low, const float high)
{
	return value >= low && value <= high;
}

float CollisionHandler::dotProduct(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}

float CollisionHandler::square(float val)
{
	return val * val;
}

bool CollisionHandler::updateEarliestCollisionDetails(CollisionDetails& earliestCollisionDetails,const CollisionDetails& collisionDetails)
{
	if (collisionDetails.timeToCollision >= 0 && collisionDetails.timeToCollision < earliestCollisionDetails.timeToCollision) {
		earliestCollisionDetails = collisionDetails;
		return true;
	}
	return false;
}

CollisionHandler::CollisionDetails CollisionHandler::handleBallToWalls()
{
	CollisionDetails earliestCollisionDetails;
	earliestCollisionDetails.timeToCollision = std::numeric_limits<float>::infinity();
	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToVerticalLine(0, 0, m_windowHeight)); // left wall
	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToVerticalLine(m_windowWidth, 0, m_windowHeight)); // right wall

	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToHorizontalLine(0, 0, m_windowWidth)); // top wall
	if (updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToHorizontalLine(m_windowHeight, 0, m_windowWidth))) { // bottom wall
		earliestCollisionDetails.collisionType = BOTTOM_WALL;
	}
	else {
		earliestCollisionDetails.collisionType = NOT_BOTTOM_WALL;
	}

	return earliestCollisionDetails;
}

CollisionHandler::CollisionDetails CollisionHandler::handleBallToPaddle()
{
	CollisionDetails earliestCollisionDetails;
	earliestCollisionDetails.timeToCollision = std::numeric_limits<float>::infinity();

	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToPoint(m_paddle.getLeftUpperCornerPos()));
	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToPoint(m_paddle.getRightUpperCornerPos()));
	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToPoint(m_paddle.getRightBottomCornerPos()));
	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToPoint(m_paddle.getLeftBottomCornerPos()));

	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToHorizontalLine(m_paddle.getBottomYBounding(), m_paddle.getLeftXBounding(), m_paddle.getRightXBounding()));
	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToHorizontalLine(m_paddle.getUpperYBounding(), m_paddle.getLeftXBounding(), m_paddle.getRightXBounding()));

	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToVerticalLine(m_paddle.getRightXBounding(), m_paddle.getUpperYBounding(), m_paddle.getBottomYBounding()));
	updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToVerticalLine(m_paddle.getLeftXBounding(), m_paddle.getUpperYBounding(), m_paddle.getBottomYBounding()));

	earliestCollisionDetails.collisionType = PADDLE;

	return earliestCollisionDetails;
}

CollisionHandler::CollisionDetails CollisionHandler::handleBallToBlocks()
{
	CollisionDetails earliestCollisionDetails;
	earliestCollisionDetails.timeToCollision = std::numeric_limits<float>::infinity();

	for (const auto& block : m_blocks) {
		float beforeEarliestCollisionTime = earliestCollisionDetails.timeToCollision;
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToPoint(block.getLeftUpperCornerPos()));
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToPoint(block.getRightUpperCornerPos()));
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToPoint(block.getRightBottomCornerPos()));
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToPoint(block.getLeftBottomCornerPos()));
			
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToHorizontalLine(block.getBottomYBounding(), block.getLeftXBounding(), block.getRightXBounding()));
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToHorizontalLine(block.getUpYBouding(), block.getLeftXBounding(), block.getRightXBounding()));
			
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToVerticalLine(block.getRightXBounding(), block.getUpYBouding(), block.getBottomYBounding()));
		updateEarliestCollisionDetails(earliestCollisionDetails, handleBallToVerticalLine(block.getLeftXBounding(), block.getUpYBouding(), block.getBottomYBounding()));
		if (earliestCollisionDetails.timeToCollision < beforeEarliestCollisionTime) {
			blockToDestroyIndex = block.m_blockNum;
		}
	}
	
	earliestCollisionDetails.collisionType = BLOCK;

	return earliestCollisionDetails;
}

void CollisionHandler::updatePaddle(float deltaTime)
{
	m_paddle.setPosition(m_paddle.getPosition() + m_paddle.getVelocity() * deltaTime);
	if (m_paddle.getRightXBounding() > m_windowWidth) {
		m_paddle.setPosition({ m_windowWidth - m_paddle.getWidth() / 2.0f, m_paddle.getPosition().y });
		return;
	}
	if (m_paddle.getLeftXBounding() < 0) {
		m_paddle.setPosition({ m_paddle.getWidth() / 2.0f, m_paddle.getPosition().y });
		return;
	}
}

CollisionHandler::CollisionDetails CollisionHandler::handleBallToVerticalLine(float lineXPos, float yLowerLimit, float yUpperLimit) {
	CollisionDetails collisionDetails;
	collisionDetails.timeToCollision = std::numeric_limits<float>::infinity();

	float timeToLine = -1.0f;
	if (m_ball.getVelocity().x < 0) { // if ball is moving left
		timeToLine = (lineXPos + m_ball.radius - m_ball.getPosition().x) / m_ball.getVelocity().x;
	}
	else if (m_ball.getVelocity().x > 0) { // if ball is moving right
		timeToLine = (lineXPos - m_ball.radius - m_ball.getPosition().x) / m_ball.getVelocity().x;
	}

	if (timeToLine < 0) {
		return collisionDetails;
	}
	float yPos = m_ball.getPosition().y + m_ball.getVelocity().y * timeToLine;
	if (!isInBounds(yPos, yLowerLimit, yUpperLimit)) {
		return collisionDetails;
	}

	collisionDetails.timeToCollision = timeToLine;
	collisionDetails.ballVelocityAfterCollision.x = -m_ball.getVelocity().x;
	collisionDetails.ballVelocityAfterCollision.y = m_ball.getVelocity().y;
	collisionDetails.ballPosAtCollision = m_ball.getPosition() + m_ball.getVelocity() * collisionDetails.timeToCollision;

	return collisionDetails;
}

CollisionHandler::CollisionDetails CollisionHandler::handleBallToHorizontalLine(float lineYPos, float xLowerLimit, float xUpperLimit) {
	CollisionDetails collisionDetails;
	collisionDetails.timeToCollision = std::numeric_limits<float>::infinity();

	float timeToLine = -1.0f;
	if (m_ball.getVelocity().y < 0) { // if ball is moving up
		timeToLine = (lineYPos + m_ball.radius - m_ball.getPosition().y) / m_ball.getVelocity().y;
	}
	else if (m_ball.getVelocity().y > 0) { // if ball is moving down
		timeToLine = (lineYPos - m_ball.radius - m_ball.getPosition().y) / m_ball.getVelocity().y;
	}

	if (timeToLine < 0) {
		return collisionDetails;
	}
	float xPos = m_ball.getPosition().x + m_ball.getVelocity().x * timeToLine;
	if (!isInBounds(xPos, xLowerLimit, xUpperLimit)) {
		return collisionDetails;
	}

	collisionDetails.timeToCollision = timeToLine;
	collisionDetails.ballVelocityAfterCollision.x = m_ball.getVelocity().x;
	collisionDetails.ballVelocityAfterCollision.y = -m_ball.getVelocity().y;
	collisionDetails.ballPosAtCollision = m_ball.getPosition() + m_ball.getVelocity() * collisionDetails.timeToCollision;

	return collisionDetails;
}

CollisionHandler::CollisionDetails CollisionHandler::handleBallToPoint(const sf::Vector2f& pointPosition)
{
	CollisionDetails collisionDetails;
	collisionDetails.timeToCollision = std::numeric_limits<float>::infinity();

	sf::Vector2f v = m_ball.getVelocity();
	auto [x_0, y_0] = m_ball.getPosition();
	const auto& [x_d, y_d] = pointPosition;
	sf::Vector2f w_b(x_0 - x_d, y_0 - y_d);

	float a = dotProduct(v, v);
	float b = 2.0f * dotProduct(w_b, v);
	float c = dotProduct(w_b, w_b) - m_ball.radius * m_ball.radius;
	float delta = b * b - 4.0f * a * c;
	if (delta >= 0) {
		float t = (-b - sqrt(delta)) / (2.0f * a);
		if (t < 0) { // point is behind ball
			return collisionDetails;
		}
		x_0 = x_0 + v.x * t;
		y_0 = y_0 + v.y * t;
		sf::Vector2f w(x_0 - x_d, y_0 - y_d);

		a = m_ball.radius * m_ball.radius;
		b = 2 * dotProduct(w, v) * w.x;
		c = square(dotProduct(w, v)) - dotProduct(v, v) * square(w.y);
		delta = b * b - 4.0f * a * c;
		if (delta < 0) {
			return collisionDetails;
		}

		sf::Vector2f u;

		// checking if vector v is parralel to vector u - if so it means that calculated u is exactly v
		// so we should search another answer which will be correct (it is due to two possible results from
		// quadratic equation)
		float ux1 = (-b + sqrt(delta)) / (2.0f * a);
		float ux2 = (-b - sqrt(delta)) / (2.0f * a);
		if (abs(ux1 + v.x) < abs(ux2 + v.x)) {
			u.x = ux2;
		}
		else {
			u.x = ux1;
		}

		u.y = -(dotProduct(w, v) + w.x * u.x) / w.y;

		collisionDetails.timeToCollision = t;
		collisionDetails.ballVelocityAfterCollision = u;
		collisionDetails.ballPosAtCollision.x = x_0;
		collisionDetails.ballPosAtCollision.y = y_0;

		return collisionDetails;
	}
	// there is no collision of point with ball
	return collisionDetails;
}
