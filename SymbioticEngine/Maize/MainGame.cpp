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
			printf("FPS: %d", (int)fps_);
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
}

void MainGame::drawGame() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start using the shader program (enable each attribute)
	shaderProgram_.startUsing();

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = shaderProgram_.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	// Set the camera matrix
	GLint cameraMatrixLocation = shaderProgram_.getUniformLocation("cameraMatrix");
	glUniformMatrix4fv(cameraMatrixLocation, 1, GL_FALSE, &camera_.getCameraMatrix()[0][0]);

	// Start using sprite batch (clears the current glyphs and batches)
	spriteBatch_.startUsing();

	/*
	
	DRAW ALL OBJECTS!!!!
	
	*/

	// Stop using the sprite batch (sorts glyphs and creates batches)
	spriteBatch_.stopUsing();
	// Render all the batches created
	spriteBatch_.renderBatch();

	

}