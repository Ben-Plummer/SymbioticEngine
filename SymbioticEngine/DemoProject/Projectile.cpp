#include "Projectile.h"

Projectile::Projectile(glm::vec2 position, glm::vec2 direction, float speed, GLuint textureID, int lifeTime) {
	position_ = position;
	direction_ = direction;
	speed_ = speed;
	textureID_ = textureID;
	lifeTime_ = lifeTime;
}

void Projectile::draw(sbe::SpriteBatch& spriteBatch) {

	spriteBatch.draw(glm::vec4(position_.x, position_.y, 30, 30), 
		             glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 
		             textureID_, sbe::Colour(255, 255, 255, 255), 1.0f);
}

bool Projectile::update() {
	position_ += direction_ * speed_;
	lifeTime_--;
	if (lifeTime_ <= 0) {
		return true;
	}
	return false;
}