#include "Enemy.h"

Enemy::Enemy() {
	sprite = NULL;
	map = NULL;
	dir = -1;
	bDelete = false, spawned = false;
	currentTime = 0;
	jumpAngle = 0, startY = 0;
	showingText = false;
	
	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font!!!" << endl;
	}
}

Enemy::~Enemy() {
	if (sprite != NULL) delete sprite;
	if (map != NULL) delete map;
}

void Enemy::render(glm::vec2& cameraPos) {
	if (!bDelete) sprite->render();

	
	if (showingText) {
		float y = timeText / TIME_TEXT * 48;
		if (timeText > TIME_TEXT) showingText = false;
		text.render("100", glm::vec2(textPos.x + cameraPos.x, textPos.y + tileMapDispl.y - y), 20, glm::vec4(1, 1, 1, 1));
	}
	
}

void Enemy::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Enemy::update(int deltaTime, float xmin, float xmax) {
	cout << "ENEMY" << endl;
}

void Enemy::setPosition(const glm::vec2& pos) {
	cout << "Enemy set position" << endl;
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

int Enemy::collision(const glm::vec2& pos, const glm::vec2& size) {
	cout << "ENEMY COLLISION" << endl;
	return 0;
}

void Enemy::setOriginalPosition(const glm::vec2& pos) {
	originalPos = pos;
}

glm::ivec2 Enemy::getPos() const {
	return posEnemy;
}

glm::ivec2 Enemy::getSize() const {
	return glm::ivec2(32, 32);
}

void Enemy::restart() {
	dir = -1;
	bDelete = false, spawned = false;
	posEnemy = originalPos;
	currentTime = 0;
	showingText = false;
}

void Enemy::changeDirection() {
	dir *= -1;
}

bool Enemy::canKillEnemies() const {
	return false;
}

void Enemy::dyingAnimation() {
	jumpAngle += JUMP_ANGLE_STEP;

	if (jumpAngle < 90) {
		posEnemy.y = int(startY - JUMP_HEIGHT_DYING * sin(3.14159f * jumpAngle / 180.f));
	}
	else {
		posEnemy.y += 4;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::kill() {}

void Enemy::showText() {
	showingText = true;
	timeText = 0;
	textPos = posEnemy;
}