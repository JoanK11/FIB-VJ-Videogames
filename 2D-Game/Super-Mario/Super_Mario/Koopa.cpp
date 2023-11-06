#include "Koopa.h"

enum koopaAnims { MOVE_LEFT, MOVE_RIGHT, SHELL, UPSIDE_DOWN };

void Koopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	state = NOT_SPAWNED;

	spritesheet.loadFromFile("images/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.125, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE_LEFT, 2);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 2);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375f, 0.5f));

	sprite->setAnimationSpeed(SHELL, 1);
	sprite->addKeyframe(SHELL, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(UPSIDE_DOWN, 1);
	sprite->addKeyframe(UPSIDE_DOWN, glm::vec2(0.75f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y - 8)));
}

void Koopa::restart() {
	Enemy::restart();
	state = NOT_SPAWNED;
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y - 8)));
}

void Koopa::update(int deltaTime, float xmin, float xmax) {
	if (showingText) timeText += deltaTime;

	if (bDelete || state == SHELL_IDLE || state == DIED) return;

	if (state == NOT_SPAWNED) {
		if (posEnemy.x <= xmax + SPAWN_DISTANCE * 32) state = MOVING;
		else return;
	}

	sprite->update(deltaTime);

	if (posEnemy.x < xmin - 32 || posEnemy.y >= map->getMapSize().y * map->getBlockSize()) {
		state = DIED;
		bDelete = true;
		return;
	}

	if (state == DYING) {
		Enemy::dyingAnimation();
		return;
	}

	glm::ivec2 dimKoopa = Koopa::getSize();

	if (map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), &posEnemy.x, true)) {
		dir = 1;
	}
	else if (map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), &posEnemy.x, true)) {
		dir = -1;
	}

	posEnemy.x += (state == MOVING ? VX : 6) * dir;

	posEnemy.y += FALL_STEP;
	if (!map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y));

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y - 32)));
	if (posEnemy.y >= map->getMapSize().y *map->getBlockSize()) bDelete = true;
}

int Koopa::collision(const glm::vec2& pos, const glm::vec2& size) {
	if (state == NOT_SPAWNED || state == DYING || (state == DIED && bDelete)) return 0;

	// Margin for collision from above
	float margin = 0.5f;

	float posL = pos.x, posR = pos.x + size.x;
	float posT = pos.y, posB = pos.y + size.y;

	float goombaL = posEnemy.x, goombaR = posEnemy.x + 32;
	float goombaT = posEnemy.y, goombaB = posEnemy.y + 32;

	// Check for collision from above (Koopa turns shell or dies)
	if (posB + margin >= goombaT && posB - margin <= goombaT &&
		posT <= goombaB && posR >= goombaL && posL <= goombaR) {
		//cout << "Mario has collided from above" << endl;
		if (state == MOVING || state == SHELL_MOVING) {
			state = SHELL_IDLE, sprite->changeAnimation(SHELL);
		}
		else if (state == SHELL_IDLE) {
			state = SHELL_MOVING;
			if (pos.x < posEnemy.x) dir = 1;
			else dir = -1;
		}

		sound.playSFX("sfx/kick.wav");
		Score::instance().increaseScore(100);
		Enemy::showText();
		return 1;
	}

	// Check for collision from the left or right
	if (posB >= goombaT && posT <= goombaB && posR >= goombaL && posL <= goombaR) {
		//cout << "Mario has collided from left or right" << endl;

		if (state == SHELL_IDLE) {
			state = SHELL_MOVING;
			if (pos.x < posEnemy.x + 16) dir = 1;
			else dir = -1;
			return 0;
		}
		return -1;
	}

	// No collision
	return 0;
}

bool Koopa::canCollide() {
	return !(state == NOT_SPAWNED || state == DYING || (state == DIED && bDelete));
}

glm::ivec2 Koopa::getSize() const {
	if (state == SHELL_IDLE || state == SHELL_MOVING)
		return glm::ivec2(32, 30);
	else return glm::ivec2(32, 48);
}

bool Koopa::canKillEnemies() const {
	if (state == SHELL_MOVING) return true;
	return false;
}

void Koopa::kill() {
	state = DYING;
	jumpAngle = 0; startY = posEnemy.y;
	sound.playSFX("sfx/kick.wav");
	Score::instance().increaseScore(100);
	sprite->changeAnimation(UPSIDE_DOWN);
	showText();
}