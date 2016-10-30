#pragma once
#include <vector>
#include "Projectile.h"
#include <SBE/Window.h>
#include <SBE/Sprite.h>
#include <SBE/ShaderProgram.h>
#include <SBE/GLTexture.h>
#include <SBE/FpsLimiter.h>
#include <SBE/InputManager.h>
#include <SBE/Camera.h>
#include <SBE/SpriteBatch.h>

enum class State { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void initGame();
	void gameLoop();
	void processInput();
	void drawGame();

	glm::ivec2 _screenDimensions;
	State _gameState;
	sbe::Window _window;
	sbe::ShaderProgram _testProgram;
	sbe::FpsLimiter _fpsLimiter;
	sbe::InputManager _inputManager;
	sbe::Camera _camera;
	sbe::SpriteBatch _spriteBatch;

	int _frameCounter;
	float _fps;

	sbe::GLTexture _playerTexture;
	sbe::GLTexture _bulletTexture;
	std::vector<Projectile> _bullets;
};

