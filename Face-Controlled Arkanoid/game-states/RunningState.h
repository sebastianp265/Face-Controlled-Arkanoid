#pragma once
#include "GameState.h"
#include <list>
#include <mutex>
#include <shared_mutex>
#include <opencv2/opencv.hpp>

#include "../CollisionHandler.h"
#include "../GameObjectsParameters.h"

#include "../game-objects/Ball.h"
#include "../game-objects/Block.h"
#include "../game-objects/Paddle.h"

class RunningState : public GameState {
public:
	RunningState(Game &game, cv::VideoCapture* cameraInput, cv::Ptr<cv::FaceDetectorYN> faceDetector);
	~RunningState();
	void handleEvent(const sf::Event& event) override;
	void update() override;
	void render() override;

	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;

private:
	GameObjectsParameters gameObjParams;

	Ball m_ball;
	std::list<Block> m_blocks;
	Paddle m_paddle;
	sf::Texture m_cameraTexture;
	sf::Sprite m_cameraSprite;

	void initBlocks();

private:
	CollisionHandler m_collisionHandler;
	sf::Clock m_clock;
	bool m_isPaused = true; // stops clock from counting while paused or first run

private:
	cv::Mat m_cameraFrameRGB;
	cv::Mat m_cameraFrameForModelRGB;

	bool m_isCameraThreadRunning = true;
	bool isCameraThreadRunning();

	std::thread m_cameraThread;
	std::mutex m_cameraRunningMutex;
	std::shared_mutex m_cameraDataMutex;

	void cameraInputThreadFunction(cv::VideoCapture* cameraCapture);

	cv::Ptr<cv::FaceDetectorYN> m_faceDetector;

	float velocityFunction(float faceAngle);

	sf::Vector2f m_leftEyePosition;
	sf::Vector2f m_rightEyePosition;

	sf::Vector2f m_leftMouthCorner;
	sf::Vector2f m_rightMouthCorner;

	sf::CircleShape m_faceDot;
};