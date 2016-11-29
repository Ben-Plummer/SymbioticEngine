#pragma once
#include <GLM/glm.hpp>

#include <SBE/SpriteBatch.h>
#include <SBE/InputManager.h>

class Player
{
public:
	Player();
	~Player();
	void initialize(glm::vec2 position, float speed, int width, sbe::Colour colour, GLuint textureID, sbe::InputManager* inputManager);
	void update(const std::vector<std::string>& levelData, int tileWidth);
	void draw(sbe::SpriteBatch& spriteBatch);

	bool collideWithLevel(const std::vector<std::string>& levelData, int tileWidth);

	glm::vec2 getPosition();
private:
	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y);
	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 position_;
	float speed_;
	int width_;
	sbe::Colour colour_;
	GLuint textureID_;
	sbe::InputManager* inputManager_ = nullptr;
	int tileWidth_ = 64;
};

