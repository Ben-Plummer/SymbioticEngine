#pragma once
#include <GLM/glm.hpp>

#include <SBE/Window.h>
#include <SBE/ShaderProgram.h>
#include <SBE/InputManager.h>
#include <SBE/SpriteBatch.h>
#include <SBE/ResourceManager.h>
#include <SBE/GLTexture.h>
#include <SBE/Camera.h>
#include <SBE/FpsLimiter.h>

#include "Level.h"
#include "Player.h"

enum class State { PLAY, EXIT };

class MainGame {
public:
	MainGame();
	void run();

private:
	void initialize();
	void initShaders();
	void initLevel();
	void gameLoop();
	void processInput();
	void drawGame();

	glm::ivec2 screenSize_ = { 1024, 768 };
	State gameState_ = State::PLAY;
	float maxFps_ = 120.0f;
	float fps_ = maxFps_;

	sbe::Window window_;
	sbe::ShaderProgram shaderProgram_;
	sbe::InputManager inputManager_;
	sbe::SpriteBatch spriteBatch_;
	sbe::FpsLimiter fpsLimiter_;
	sbe::Camera camera_;

	std::vector<Level> levels_;
	int currentLevel_ = 0;

	Player player_;
	sbe::GLTexture entityTexture_;
	
	
};
