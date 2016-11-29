#pragma once
#include <SBE/SpriteBatch.h>

#include <string>
#include <vector>


class Level
{
public:
	Level(const std::string& levelPath, int tileWidth, int tileHeight);
	void draw();

	// Getters
	unsigned int getWidth() const { return levelData_[0].size(); }
	unsigned int getHeight() const { return levelData_.size(); }
	const std::vector<std::string>& getLevelData() const { return levelData_; }
	glm::vec2 getStartPlayerPos() const { return playerStartPosition_; }
	const std::vector<glm::vec2>& getRedEnemyStartPositions() const { return redEnemyStartPositions_; }
	const std::vector<glm::vec2>& getBlueEnemyStartPositions() const { return blueEnemyStartPositions_; }
private:
	std::vector<std::string> levelData_;
	sbe::SpriteBatch levelSpriteBatch_;

	glm::vec2 playerStartPosition_ = { 1.0f, 1.0f };
	std::vector<glm::vec2> redEnemyStartPositions_;
	std::vector<glm::vec2> blueEnemyStartPositions_;
};