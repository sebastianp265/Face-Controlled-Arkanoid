#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <opencv2/opencv.hpp>

#include "game-states/GameState.h"

class GameState;

class Game {
public:
    Game();
    ~Game();
    void run();
    void setState(GameState* state);

    sf::RenderWindow& getWindow();
    float getWindowWidth();
    float getWindowHeight();

    const sf::Font& getFont();
    cv::VideoCapture* getCameraCapture();

    cv::Ptr<cv::FaceDetectorYN> getFaceDetector();

    void exitGame();
    bool isRunning();

private:
    void handleEvents();

private:
    cv::VideoCapture m_cameraCapture;
    cv::Ptr<cv::FaceDetectorYN> m_faceDetector;
    sf::RenderWindow m_window;

    GameState* m_currentState;
    bool m_isRunning;
    sf::Font m_font;

public:
    const cv::Size INPUT_MODEL_SIZE;
};
