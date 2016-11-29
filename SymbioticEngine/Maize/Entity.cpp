#include "Entity.h"
#include <algorithm>

Entity::Entity()
{
}


Entity::~Entity()
{
}

void Entity::initialize(glm::vec2 position, float speed, int width, sbe::Colour colour, GLuint textureID) {
	position_ = position;
	width_ = width;
	colour_ = colour;
	textureID_ = textureID;
}

void Entity::draw(sbe::SpriteBatch& spriteBatch) {
	glm::vec4 destRect(position_.x, position_.y, width_, width_);
	spriteBatch.draw(destRect, textureID_, colour_);
}

bool Entity::collideWithLevel(const std::vector<std::string>& levelData, int tileWidth) {
	std::vector<glm::vec2> collideTilePositions;
	tileWidth_ = tileWidth;

	// Check the four corners
	// First corner
	checkTilePosition(levelData,
		collideTilePositions,
		position_.x,
		position_.y);
	// Second Corner
	checkTilePosition(levelData,
		collideTilePositions,
		position_.x + width_,
		position_.y);

	// Third Corner
	checkTilePosition(levelData,
		collideTilePositions,
		position_.x,
		position_.y + width_);

	// Third Corner
	checkTilePosition(levelData,
		collideTilePositions,
		position_.x + width_,
		position_.y + width_);
	
	// Check there were collisions
	if (collideTilePositions.empty()) {
		return false;
	}

	// Do the collision
	for (unsigned int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
	return true;
}

void Entity::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y) {
	

	// Get the position of this corner in grid space
	glm::vec2 gridPos = glm::vec2(floor(x / (float)tileWidth_), floor(y / (float)tileWidth_));

	// If we are outside the world, just return
	if (gridPos.x < 0 || gridPos.x >= levelData[0].size() ||
		gridPos.y < 0 || gridPos.y >= levelData.size()) {
		return;
	}

	// If this is not an air tile, we should collide with it
	if (levelData[gridPos.y][gridPos.x] != '.') {
		collideTilePositions.push_back(gridPos * (float)tileWidth_ + glm::vec2((float)tileWidth_ / 2.0f));
	}
}

void Entity::collideWithTile(glm::vec2 tilePos) {
	const float TILE_RADIUS = (float)tileWidth_ / 2.0f;
	// The minimum distance before a collision occurs
	const float MIN_DISTANCE = width_ / 2 + TILE_RADIUS;

	// Center position of the agent
	glm::vec2 centerAgentPos = position_ + glm::vec2(width_ / 2);
	// Vector from the agent to the tile
	glm::vec2 distVec = centerAgentPos - tilePos;

	// Get the depth of the collision
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	// If both the depths are > 0, then we collided
	if (xDepth > 0 && yDepth > 0) {

		// Check which collision depth is less
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			// X collsion depth is smaller so we push in X direction
			if (distVec.x < 0) {
				position_.x -= xDepth;
			}
			else {
				position_.x += xDepth;
			}
		}
		else {
			// Y collsion depth is smaller so we push in X direction
			if (distVec.y < 0) {
				position_.y -= yDepth;
			}
			else {
				position_.y += yDepth;
			}
		}
	}
}