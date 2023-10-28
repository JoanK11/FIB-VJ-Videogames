#include "Goomba.h"

#define FALL_STEP 4
#define VX 1

enum GoombaAnims {MOVE, DIED};

void Goomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	died = false;

	spritesheet.loadFromFile("images/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.25	), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(MOVE, 2);
		sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE, glm::vec2(0.125f, 0.f));

		sprite->setAnimationSpeed(DIED, 1);
		sprite->addKeyframe(DIED, glm::vec2(0.25f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Goomba::update(int deltaTime, float xmin) {
	if (bDelete) return;
	sprite->update(deltaTime);

	if (died) {
		if (sprite->animation() != DIED)
			sprite->changeAnimation(DIED);
		return;
	}

	if (posEnemy.x < xmin - 32) {
		bDelete = true;
		return;
	}

	if (map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), &posEnemy.x, false)) {
	}
		

	if (map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), &posEnemy.x, false) && dir < 0) {
		dir = 1;
	}	
	else if (map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), &posEnemy.x, false) && dir > 0) {
		dir = -1;
	}

	posEnemy.x += VX * dir;

	posEnemy.y += FALL_STEP;
	if (!map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y));

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

int Goomba::collision(const glm::vec2& pos, const glm::vec2& size) {
	if (died) return 0;

	// Margin for collision from above
	float margin = 3.0f;

	float posL = pos.x, posR = pos.x + size.x;
	float posT = pos.y, posB = pos.y + size.y;

	float goombaL = posEnemy.x, goombaR = posEnemy.x + 32;
	float goombaT = posEnemy.y, goombaB = posEnemy.y + 32;

	// Check for collision from above (Goomba dies)
	if (posB + margin >= goombaT && posB - margin <= goombaT &&
		posT <= goombaB && posR >= goombaL && posL <= goombaR) {
		cout << "Mario has collided from above" << endl;
		died = true;
		Score::instance().increaseScore(100);
		return 1;
	}

	// Check for collision from the left or right
	if (posB >= goombaT && posT <= goombaB && posR >= goombaL && posL <= goombaR) {
		cout << "Mario has collided from left or right" << endl;
		return -1;
	}

	// No collision
	return 0;
}

void Goomba::setPosition(const glm::vec2& pos) {
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}