#include "RunningState.h"
#include "PauseState.h"
#include "GameResultState.h"
#include "MenuState.h"

#include <numbers>

RunningState::RunningState(Game& game, cv::VideoCapture* cameraCapture, cv::Ptr<cv::FaceDetectorYN> faceDetector) : GameState(game),
	gameObjParams(m_game.getWindowWidth(), m_game.getWindowHeight()),
	m_ball(gameObjParams.BALL_START_POSITION, gameObjParams.BALL_RADIUS),
	m_paddle(gameObjParams.PADDLE_STARTING_POSITION, gameObjParams.PADDLE_SIZE),
	m_collisionHandler(m_ball, m_paddle, m_blocks, m_game.getWindowWidth(), m_game.getWindowHeight()),
	m_cameraThread(&RunningState::cameraInputThreadFunction, this, cameraCapture),
	m_faceDetector(faceDetector),
	WINDOW_WIDTH(static_cast<int>(m_game.getWindowWidth())),
	WINDOW_HEIGHT(static_cast<int>(m_game.getWindowHeight()))
{
	const float faceDotRadius = 5.0f;
	m_faceDot.setRadius(faceDotRadius);
	m_faceDot.setOrigin(faceDotRadius / 2.0f, faceDotRadius / 2.0f);
	initBlocks();
	m_ball.setVelocity({ 200.0f, 200.0f });
	m_cameraTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
}

RunningState::~RunningState()
{
	{
		std::lock_guard<std::mutex> lock(m_cameraRunningMutex);
		m_isCameraThreadRunning = false;
	}
	m_cameraThread.join();
}

bool RunningState::isCameraThreadRunning()
{
	std::lock_guard<std::mutex> lock(m_cameraRunningMutex);
	return m_isCameraThreadRunning;
}

void RunningState::cameraInputThreadFunction(cv::VideoCapture* cameraCapture)
{
	
	while (isCameraThreadRunning()) {
		cv::Mat cameraFrameForModelRGB;
		cv::Mat cameraFrameRGB;
		*cameraCapture >> cameraFrameRGB;
		if (!cameraFrameRGB.empty()) {
			cv::flip(cameraFrameRGB, cameraFrameRGB, 1);
			cv::resize(cameraFrameRGB, cameraFrameForModelRGB, m_game.INPUT_MODEL_SIZE);
			cv::resize(cameraFrameRGB, cameraFrameRGB, cv::Size(WINDOW_WIDTH, WINDOW_HEIGHT));
			{
				std::shared_lock<std::shared_mutex> lock(m_cameraDataMutex);
				m_cameraFrameRGB = cameraFrameRGB.clone();
				m_cameraFrameForModelRGB = cameraFrameForModelRGB.clone();
			}/*
			cv::Mat cameraFrameRGBA;
			cv::cvtColor(cameraFrameRGB, cameraFrameRGBA, cv::COLOR_BGR2RGBA);

			texture.update(cameraFrameRGBA.data);
			m_cameraSprite.setTexture(texture);*/

			//faceDetector->detect(cameraFrameRGB, m_faces);
		}
	}

}

void RunningState::initBlocks()
{
	m_blocks.clear();
	for (size_t i = 0; i < gameObjParams.BLOCK_ROWS; i++) {
		for (size_t j = 0; j < gameObjParams.BLOCK_COLUMNS; j++) {
			Block block({ j * (gameObjParams.BLOCK_WIDTH + gameObjParams.BLOCK_MARGIN) + gameObjParams.BLOCK_FIELD_MARGIN + gameObjParams.BLOCK_WIDTH / 2.0f,
				i * (gameObjParams.BLOCK_HEIGHT + gameObjParams.BLOCK_MARGIN) + gameObjParams.BLOCK_FIELD_MARGIN + gameObjParams.BLOCK_HEIGHT / 2.0f },
				gameObjParams.BLOCK_WIDTH,
				gameObjParams.BLOCK_HEIGHT,
				sf::Color::Magenta,
				i * gameObjParams.BLOCK_COLUMNS + j);
			m_blocks.push_back(block);
		}
	}
}

void RunningState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			m_isPaused = true;
			m_game.setState(new PauseState(m_game, this));
		}
		else if (event.key.code == sf::Keyboard::Right) {
			m_paddle.setXVelocity(400.0f);
		}
		else if (event.key.code == sf::Keyboard::Left) {
			m_paddle.setXVelocity(-400.0f);
		}
	}
	else if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left) {
			m_paddle.setXVelocity(0.0f);
		}
	}
}

void RunningState::update()
{
	float deltaTime;
	if (m_isPaused) {
		deltaTime = 0.0f;
		m_isPaused = false;
		m_clock.restart();
	}
	else {
		deltaTime = m_clock.restart().asSeconds();
	}
	while (!m_cameraDataMutex.try_lock());
	cv::Mat faces;
	if (!m_cameraFrameRGB.empty()) {
		m_faceDetector->detect(m_cameraFrameForModelRGB, faces);
		if (faces.rows == 1) {
			m_leftEyePosition = sf::Vector2f(faces.at<float>(0, 4) * WINDOW_WIDTH / m_game.INPUT_MODEL_SIZE.width
				, faces.at<float>(0, 5) * WINDOW_HEIGHT / m_game.INPUT_MODEL_SIZE.height);
			m_rightEyePosition = sf::Vector2f(faces.at<float>(0, 6) * WINDOW_WIDTH / m_game.INPUT_MODEL_SIZE.width
				, faces.at<float>(0, 7) * WINDOW_HEIGHT / m_game.INPUT_MODEL_SIZE.height);

			m_leftMouthCorner = sf::Vector2f(faces.at<float>(0, 10) * WINDOW_WIDTH / m_game.INPUT_MODEL_SIZE.width
				, faces.at<float>(0, 11) * WINDOW_HEIGHT / m_game.INPUT_MODEL_SIZE.height);
			m_rightMouthCorner = sf::Vector2f(faces.at<float>(0, 12) * WINDOW_WIDTH / m_game.INPUT_MODEL_SIZE.width
				, faces.at<float>(0, 13) * WINDOW_HEIGHT / m_game.INPUT_MODEL_SIZE.height);

			sf::Vector2f leftSide((m_leftEyePosition + m_leftMouthCorner) / 2.0f);
			sf::Vector2f rightSide((m_rightEyePosition + m_rightMouthCorner) / 2.0f);

			float faceAngle = atan2(leftSide.y - rightSide.y, leftSide.x - rightSide.x);
			faceAngle = (faceAngle * 180.0f) / std::numbers::pi_v<float>;
			faceAngle -= std::copysign(1.0f, faceAngle) * 180.0f;
			m_paddle.setXVelocity(velocityFunction(faceAngle));
		}
	}

	bool isGameLost = m_collisionHandler.update(deltaTime);
	if (isGameLost || m_blocks.size() == 0) {
		m_cameraDataMutex.unlock();
		m_game.setState(new GameResultState(m_game, m_blocks.size() == 0));
		delete this;
	}
}

void RunningState::render()
{
	sf::RenderWindow& renderWindow = m_game.getWindow();
	
	if (!m_cameraFrameRGB.empty()) {
		cv::Mat cameraFrameRGBA;
		cv::cvtColor(m_cameraFrameRGB, cameraFrameRGBA, cv::COLOR_BGR2RGBA);
		m_cameraTexture.update(cameraFrameRGBA.data);
		m_cameraSprite.setTexture(m_cameraTexture);
		renderWindow.draw(m_cameraSprite);

		// rendering points on face
		m_faceDot.setFillColor(sf::Color::Red);

		m_faceDot.setPosition(m_leftEyePosition);
		renderWindow.draw(m_faceDot);
		m_faceDot.setPosition(m_leftMouthCorner);
		renderWindow.draw(m_faceDot);

		m_faceDot.setFillColor(sf::Color::Green);

		m_faceDot.setPosition(m_rightEyePosition);
		renderWindow.draw(m_faceDot);
		m_faceDot.setPosition(m_rightMouthCorner);
		renderWindow.draw(m_faceDot);
	}
	m_cameraDataMutex.unlock();

	renderWindow.draw(m_ball);
	renderWindow.draw(m_paddle);
	for (auto& block : m_blocks) {
		renderWindow.draw(block);
	}
}


float RunningState::velocityFunction(float faceAngle)
{
	const float activationAngle = 15.0f;
	const float constantSpeedActivationAngle = 50.0f;

	float a = 300.0f;
	float velocity = 0.0f;
	if (abs(faceAngle) > activationAngle) {

		velocity = a * log10f(abs(faceAngle) - activationAngle + 1);
		//velocity = 300.0f;
		if (faceAngle < 0) {
			velocity *= -1;
		}
		return velocity;
	}
	else {
		return velocity;
	}
}

