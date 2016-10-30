#include "Level.h"
#include <fstream>

#include <SBE/Error.h>
#include <SBE/ResourceManager.h>

Level::Level(const std::string& levelPath, int tileWidth, int tileHeight) {
	// Open text file from level path
	std::ifstream levelFile;
	levelFile.open(levelPath);

	// Checks if file was opened
	if (levelFile.fail()) {
		sbe::fatalError("Failed to open level file from " + levelPath);
	}

	// Temporary variable to line contents
	std::string tmp;
	// Read the level data
	while (std::getline(levelFile, tmp)) {
		levelData_.emplace_back(tmp);
	}

	// Initialize and start using sprite batch for the level
	levelSpriteBatch_.initialize();
	levelSpriteBatch_.startUsing();

	//Render the tiles
	for (unsigned int y = 0; y < levelData_.size(); y++) {
		for (unsigned int x = 0; x < levelData_[y].size(); x++) {
			//Get the tile at line y at index x
			char tile = levelData_[y][x];

			//Set up dest rect using tile width and height and position in txt file
			glm::vec4 tileRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);

			//Check which tile it is and then draw accordingly
			switch (tile) {
			case 'W':
				levelSpriteBatch_.draw(tileRect, sbe::ResourceManager::getTexture("Textures/WhiteTile.png").id);
				break;
			case 'G':
				levelSpriteBatch_.draw(tileRect, sbe::ResourceManager::getTexture("Textures/GreenTile.png").id);
				break;
			case 'P':
				levelSpriteBatch_.draw(tileRect, sbe::ResourceManager::getTexture("Textures/PurpleTile.png").id);
				break;
			case 'O':
				levelSpriteBatch_.draw(tileRect, sbe::ResourceManager::getTexture("Textures/OrangeTile.png").id);
				break;
			case 'L':
				levelData_[y][x] = '.'; /// So we dont collide with L
				playerStartPosition_.x = x * tileWidth;
				playerStartPosition_.y = y * tileHeight;
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol '%c' found in level file %s at line %d character %d", tile, levelPath, x, y);
				break;
			}
		}
	}
	// Generate glyphs and batches
	levelSpriteBatch_.stopUsing();
}

void Level::draw() {
	levelSpriteBatch_.renderBatch();
}