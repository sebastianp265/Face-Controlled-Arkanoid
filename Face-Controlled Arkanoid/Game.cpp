#include "Game.h"
#include "game-states/MenuState.h"

#include <iostream>

Game::Game() : m_window(sf::VideoMode(1080, 720), "Face-Controlled Arkanoid", sf::Style::Titlebar | sf::Style::Close),
m_cameraCapture(0),
INPUT_MODEL_SIZE(320, m_window.getSize().y / (m_window.getSize().x / 320))
{
	if (!m_font.loadFromFile("UI/ARIAL.TTF")) {
		std::cout << "COULDN'T READ FONT";
	}
	m_window.setFramerateLimit(140);
	m_faceDetector = cv::FaceDetectorYN::create("face_detection_yunet_2022mar.onnx", "", INPUT_MODEL_SIZE);;
}

Game::~Game()
{
	m_window.close();
	m_cameraCapture.release();
}

void Game::run()
{
	m_currentState = new MenuState(*this);
	m_isRunning = true;
	while (m_isRunning) {
		m_window.clear(sf::Color::Black);
		handleEvents();
		m_currentState->update();
		m_currentState->render();
		m_window.display();
	}
	delete m_currentState;
}

void Game::setState(GameState* state)
{
	m_currentState = state;
}

sf::RenderWindow& Game::getWindow()
{
	return m_window;
}

float Game::getWindowWidth()
{
	return static_cast<float>(m_window.getSize().x);
}

float Game::getWindowHeight()
{
	return static_cast<float>(m_window.getSize().y);
}

const sf::Font& Game::getFont()
{
	return m_font;
}

cv::VideoCapture* Game::getCameraCapture()
{
	return &m_cameraCapture;
}

cv::Ptr<cv::FaceDetectorYN> Game::getFaceDetector()
{
	return m_faceDetector;
}

void Game::exitGame()
{
	m_isRunning = false;
}

bool Game::isRunning()
{
	return m_isRunning;
}

void Game::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_isRunning = false;
			break;
		}
		m_currentState->handleEvent(event);
	}
}
