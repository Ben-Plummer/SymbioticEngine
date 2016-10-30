#pragma once
#include <GLM/glm.hpp>
#include <SBE/SpriteBatch.h>
#include <SBE/GLTexture.h>

class Projectile
{
public:
	Projectile(glm::vec2 position, glm::vec2 direction, float speed, GLuint textureID, int lifeTime);
	void draw(sbe::SpriteBatch& spriteBatch);
	bool update();
private:
	glm::vec2 position_;
	glm::vec2 direction_;
	float speed_;
	GLuint textureID_;
	int lifeTime_;
};

