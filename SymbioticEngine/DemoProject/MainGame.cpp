#include "MainGame.h"
#include <GL/glew.h>
#include <SBE/ResourceManager.h>
#include <math.h>

MainGame::MainGame() :
	_screenDimensions(1024, 768),
	_gameState(State::PLAY),
	_frameCounter(0),
	_fps(0) {
	_camera.init(_screenDimensions.x, _screenDimensions.y);
}

MainGame::~MainGame() {

}

void MainGame::run() {
	initSystems();
	initGame();
	
	gameLoop();
}

void MainGame::initSystems() {
	_window.create("Graphics Engine", _screenDimensions.x, _screenDimensions.y, 0);
	initShaders();
	_fpsLimiter.setMaxFPS(120.0f);
	_spriteBatch.initialize();
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClearDepth(1.0f);
}

void MainGame::initShaders() {
	_testProgram.compileShaders("Shaders/VertexShader.vert", "Shaders/FragmentShader.frag");
	_testProgram.addAttribute("vertexPosition");
	_testProgram.addAttribute("vertexColour");
	_testProgram.addAttribute("vertexUV");
	_testProgram.linkShaders();
}

void MainGame::initGame() {
	_playerTexture = sbe::ResourceManager::getTexture("Textures/Sprites/Player1/p1_stand.png");
	_bulletTexture = sbe::ResourceManager::getTexture("Textures/Sprites/Player1/p1_stand.png");
}

void MainGame::gameLoop() {
	while (_gameState != State::EXIT) {
		_fpsLimiter.start();
		
		_inputManager.update();

		processInput();
		drawGame();

		_camera.update();

		//Update bullets
		for (unsigned int i = 0; i < _bullets.size();) {
			if (_bullets[i].update()) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}

		_frameCounter++;
		if (_frameCounter >= 40) {
			//printf("FPS: %d\n", (int)_fps);
			_frameCounter = 0;
		}
		
		_fps = _fpsLimiter.stop();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = State::EXIT;
			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym) {
			case SDLK_w:
				_inputManager.pressKey(SDLK_w);
				break;
			case SDLK_a:
				_inputManager.pressKey(SDLK_a);
				break;
			case SDLK_s:
				_inputManager.pressKey(SDLK_s);
				break;
			case SDLK_d:
				_inputManager.pressKey(SDLK_d);
				break;
			case SDLK_q:
				_inputManager.pressKey(SDLK_q);
				break;
			case SDLK_e:
				_inputManager.pressKey(SDLK_e);
				break;
			}
			break;
		case SDL_KEYUP:
			switch (evnt.key.keysym.sym) {
			case SDLK_w:
				_inputManager.releaseKey(SDLK_w);
				break;
			case SDLK_a:
				_inputManager.releaseKey(SDLK_a);
				break;
			case SDLK_s:
				_inputManager.releaseKey(SDLK_s);
				break;
			case SDLK_d:
				_inputManager.releaseKey(SDLK_d);
				break;
			case SDLK_q:
				_inputManager.releaseKey(SDLK_q);
				break;
			case SDLK_e:
				_inputManager.releaseKey(SDLK_e);
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	const float CAMERA_SPEED = 1.0f;
	const float SCALE_SPEED = 0.01f;

	if (_inputManager.isKeyDown(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() - glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyDown(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() - glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		_inputManager.setMouseCoordinates((float)evnt.motion.x, (float)evnt.motion.y);
		glm::vec2 mouseCoordinates = _inputManager.getMouseCoordinates();
		mouseCoordinates = _camera.getWorldFromScreen(mouseCoordinates);
		mouseCoordinates += glm::vec2(1024 / 2, 768 / 2);
		glm::vec2 playerPosition(0.0);
		glm::vec2 direction = mouseCoordinates - playerPosition;
		/*
		int dirMag = abs(direction.x) + abs(direction.y);
		direction = glm::vec2(direction.x / dirMag, direction.y / dirMag);
		*/
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 10.0f, _bulletTexture.id, 60);
	}

}

void MainGame::drawGame() {
	//Clears screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_testProgram.startUsing();

	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _testProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//Set the camera matrix
	GLint cameraMatrixLocation = _testProgram.getUniformLocation("cameraMatrix");
	glUniformMatrix4fv(cameraMatrixLocation, 1, GL_FALSE, &_camera.getCameraMatrix()[0][0]);


	_spriteBatch.startUsing();
	
	glm::vec4 pos(0.0f, 0.0f, 66.0f, 92.0f);
	
	_spriteBatch.draw(pos, _playerTexture.id);
	
	for (unsigned int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.stopUsing();
	_spriteBatch.renderBatch();
	

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	_testProgram.stopUsing();

	//Swaps window buffer
	_window.swapBuffer();
}