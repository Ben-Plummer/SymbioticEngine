#include "MainGame.h"
#include <SDL/SDL.h>

MainGame::MainGame() {

}

void MainGame::run() {
	// Initialize
	initialize();
	// Start the game loop
	gameLoop();
}

void MainGame::initialize() {
	// Create window
	window_.create("MAIZE", screenSize_.x, screenSize_.y, 0);
	// Initialize shaders
	initShaders();
	// Set max fps to 120
	fpsLimiter_.setMaxFPS(maxFps_);
	// Initialize sprite batch
	spriteBatch_.initialize();
	// Set the colour the screen is cleared to, to dark blue
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	// Set the depth the screen is cleared to
	glClearDepth(1.0f);
	// Initialize game levels and objects
	initLevel();
}

void MainGame::initShaders() {
	// Compile shaders from paths
	shaderProgram_.compileShaders("Shaders/VertexShader.vert", "Shaders/FragmentShader.frag");
	// Add attributes that will be passed into shaders
	shaderProgram_.addAttribute("vertexPosition");
	shaderProgram_.addAttribute("vertexColour");
	shaderProgram_.addAttribute("vertexUV");
	// Link the shaders together in one program
	shaderProgram_.linkShaders();
}

void MainGame::initLevel() {
	levels_.push_back(Level("Levels/FirstLevel.txt", 64, 64));
}

void MainGame::gameLoop() {
	// Only continue while the game state is not set to exit
	while (gameState_ != State::EXIT) {
		// Check ticks at the start of cycle
		fpsLimiter_.start();
		// Process input from user
		processInput();
		// Draw the game
		drawGame();
		// Check ticks at end and store in fps_
		fps_ = fpsLimiter_.stop();
		// Count frames until 40 frames have been then print fps and set to 0
		static int frameCounter;
		if (frameCounter++ == 40) {
			printf("FPS: %d\n", (int)fps_);
			frameCounter = 0;
		}
	}
}

void MainGame::processInput() {
	// Create an event
	SDL_Event evnt;
	// Loop through the events when there are events
	while (SDL_PollEvent(&evnt)) {
		//Update inputManager_ depending on what keys are pressed
		switch (evnt.type) {
		case SDL_QUIT:
			gameState_ = State::EXIT;
			break;
		case SDLK_DOWN:
			switch (evnt.key.keysym.sym) {
			case SDLK_w:
				inputManager_.pressKey(SDLK_w);
				break;
			case SDLK_a:
				inputManager_.pressKey(SDLK_a);
				break;
			case SDLK_s:
				inputManager_.pressKey(SDLK_s);
				break;
			case SDLK_d:
				inputManager_.pressKey(SDLK_d);
				break;
			}
		case SDLK_UP:
			switch (evnt.key.keysym.sym) {
			case SDLK_w:
				inputManager_.releaseKey(SDLK_w);
				break;
			case SDLK_a:
				inputManager_.releaseKey(SDLK_a);
				break;
			case SDLK_s:
				inputManager_.releaseKey(SDLK_s);
				break;
			case SDLK_d:
				inputManager_.releaseKey(SDLK_d);
				break;
			}
		}
	}
	const float CAMERA_SPEED = 1.0f;
	const float SCALE_SPEED = 0.01f;

	if (inputManager_.isKeyDown(SDLK_w)) {
		camera_.setPosition(camera_.getPosition() - glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (inputManager_.isKeyDown(SDLK_a)) {
		camera_.setPosition(camera_.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (inputManager_.isKeyDown(SDLK_s)) {
		camera_.setPosition(camera_.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (inputManager_.isKeyDown(SDLK_d)) {
		camera_.setPosition(camera_.getPosition() - glm::vec2(CAMERA_SPEED, 0.0f));
	}
}

void MainGame::drawGame() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start using the shader program (enable each attribute)
	shaderProgram_.startUsing();

	// Sets the active texture to the first one
	glActiveTexture(GL_TEXTURE0);
	// Passes this into the shader program
	GLint textureLocation = shaderProgram_.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	// Set the camera matrix
	GLint cameraMatrixLocation = shaderProgram_.getUniformLocation("cameraMatrix");
	glUniformMatrix4fv(cameraMatrixLocation, 1, GL_FALSE, &camera_.getCameraMatrix()[0][0]);

	levels_[currentLevel_].draw();

	// Start using sprite batch (clears the current glyphs and batches)
	spriteBatch_.startUsing();

	/*
	
	DRAW ALL OBJECTS!!!!
	
	*/

	// Stop using the sprite batch (sorts glyphs and creates batches)
	spriteBatch_.stopUsing();
	// Render all the batches created
	spriteBatch_.renderBatch();

	//Stop using the shader program
	shaderProgram_.stopUsing();

	// Swap the buffer to prevent flickering
	window_.swapBuffer();
}