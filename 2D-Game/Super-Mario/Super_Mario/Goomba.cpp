#include "Goomba.h"

#define DIED_TIME 300.f

enum GoombaAnims {MOVE, DIED, UPSIDE_DOWN};

void Goomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	died = false, dying = false;

	spritesheet.loadFromFile("images/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.25	), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);
		
		sprite->setAnimationSpeed(MOVE, 2);
		sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE, glm::vec2(0.125f, 0.f));

		sprite->setAnimationSpeed(DIED, 1);
		sprite->addKeyframe(DIED, glm::vec2(0.25f, 0.f));

		sprite->setAnimationSpeed(UPSIDE_DOWN, 2);
		sprite->addKeyframe(UPSIDE_DOWN, glm::vec2(0.375f, 0.f));
		sprite->addKeyframe(UPSIDE_DOWN, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Goomba::restart() {
	Enemy::restart();
	died = false, dying = false;
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Goomba::update(int deltaTime, float xmin, float xmax) {
	if (showingText) timeText += deltaTime;

	if (bDelete) return;

	/* Spawn Enemy */
	if (!spawned) {
		if (posEnemy.x <= xmax + SPAWN_DISTANCE*32) spawned = true;
		else return;
	}

	/* Delete Goomba */
	if (posEnemy.x < xmin - 32 || posEnemy.y >= map->getMapSize().y * map->getBlockSize()) {
		bDelete = true;
		return;
	}

	sprite->update(deltaTime);

	/* Goomba Animation Dying and Falling off the map */
	if (dying) {
		Enemy::dyingAnimation();
		return;
	}

	/* Goomba Killed by Player */
	if (died) {
		currentTime += deltaTime;
		if (currentTime >= DIED_TIME) {
			bDelete = true;
		}
		if (sprite->animation() != DIED) {
			sprite->changeAnimation(DIED);
		}
		return;
	}

	if (map->colisionDown(posEnemy, glm::ivec2(32, 32))) {
		kill();
		return;
	} 

	/* Collisions */
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
	if (posEnemy.y >= map->getMapSize().y * map->getBlockSize()) bDelete = true;
}

int Goomba::collision(const glm::vec2& pos, const glm::vec2& size, bool isPlayer) {
	if (died || dying || bDelete || !spawned) return 0;

	// Margin for collision from above
	float margin = 0.5f;

	float posL = pos.x, posR = pos.x + size.x;
	float posT = pos.y, posB = pos.y + size.y;

	float goombaL = posEnemy.x, goombaR = posEnemy.x + 32;
	float goombaT = posEnemy.y, goombaB = posEnemy.y + 32;

	// Check for collision from above (Goomba dies)
	if (posB + margin >= goombaT && posB - margin <= goombaT &&
		posT <= goombaB && posR >= goombaL && posL <= goombaR) {
		//cout << "Mario has collided from above" << endl;
		died = true;
		sound.playSFX("sfx/kick.wav");
		Score::instance().increaseScore(100);
		Enemy::showText();
		return 1;
	}

	// Check for collision from the left or right
	if (posB >= goombaT && posT <= goombaB && posR >= goombaL && posL <= goombaR) {
		//cout << "Mario has collided from left or right" << endl;
		return -1;
	}

	// No collision
	return 0;
}

bool Goomba::canCollide(float xmin, float xmax) {
	return !(died || dying || bDelete || !spawned) && (posEnemy.x <= 2 * xmax - xmin + 320);
}

void Goomba::kill() {
	dying = true;
	jumpAngle = 0; startY = posEnemy.y;
	sound.playSFX("sfx/kick.wav");
	Score::instance().increaseScore(100);
	sprite->changeAnimation(UPSIDE_DOWN);
	
	if (posEnemy.y < map->getMapSize().y * map->getBlockSize())
		showText();
}