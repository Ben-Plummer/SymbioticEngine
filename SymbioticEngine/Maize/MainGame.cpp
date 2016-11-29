#include "MainGame.h"
#include <SDL/SDL.h>

int ENTITY_WIDTH = 48;
int TILE_WIDTH = 64;

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
	window_.create("MAZE", screenSize_.x, screenSize_.y, 0);
	// Initialize shaders
	initShaders();
	// Set max fps to 120
	fpsLimiter_.setMaxFPS(maxFps_);
	// Initialize sprite batch
	spriteBatch_.initialize();
	// Initialize the camera
	camera_.init(screenSize_.x, screenSize_.y);
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
	levels_.emplace_back("Levels/FirstLevel.txt", TILE_WIDTH, TILE_WIDTH);
	entityTexture_ = sbe::ResourceManager::getTexture("Textures/EntityModel.png");
	// Creates player
	player_.initialize(levels_[0].getStartPlayerPos(), 1.0f, ENTITY_WIDTH, sbe::Colour(0, 255, 0, 255), entityTexture_.id, &inputManager_);

}

void MainGame::gameLoop() {
	// Only continue while the game state is not set to exit
	while (gameState_ != State::EXIT) {
		// Check ticks at the start of cycle
		fpsLimiter_.start();

		//Update the input manager
		inputManager_.update();

		// Process input from user
		processInput();

		// Draw the game
		drawGame();

		// Update the camera
		camera_.update();
		// Set the position of the camera to the player
		camera_.setPosition(player_.getPosition() - glm::vec2(screenSize_.x / 2 - 24, screenSize_.y / 2 - 24));

		// Updates player
		player_.update(levels_[0].getLevelData(), TILE_WIDTH);
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
		case SDL_KEYDOWN:
			inputManager_.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager_.releaseKey(evnt.key.keysym.sym);
			break;
		}
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

	player_.draw(spriteBatch_);

	// Stop using the sprite batch (sorts glyphs and creates batches)
	spriteBatch_.stopUsing();
	// Render all the batches created
	spriteBatch_.renderBatch();

	//Stop using the shader program
	shaderProgram_.stopUsing();

	// Swap the buffer to prevent flickering
	window_.swapBuffer();
}