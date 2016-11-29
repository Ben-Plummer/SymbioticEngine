#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

#include <SBE/SpriteBatch.h>

class Entity
{
public:
	Entity();
	virtual ~Entity();
	void initialize(glm::vec2 position, float speed, int width, sbe::Colour colour, GLuint textureID);
	void draw(sbe::SpriteBatch& spriteBatch);

	bool collideWithLevel(const std::vector<std::string>& levelData, int tileWidth);
	

protected:
	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y);
	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 position_;
	int width_;
	sbe::Colour colour_;
	GLuint textureID_;
	float speed_;
	int tileWidth_;
};

