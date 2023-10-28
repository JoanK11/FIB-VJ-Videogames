#include "Enemy.h"

Enemy::Enemy() {
	sprite = NULL;
	map = NULL;
	dir = -1;
	bDelete = false;
}

Enemy::~Enemy() {
	if (sprite != NULL) delete sprite;
	if (map != NULL) delete map;
}

void Enemy::render() {
	if (bDelete) return;
	sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Enemy::update(int deltaTime, float xmin) {
	cout << "ENEMY" << endl;
}

void Enemy::setPosition(const glm::vec2& pos) {
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

int Enemy::collision(const glm::vec2& pos, const glm::vec2& size) {
	cout << "ENEMY COLLISION" << endl;
	return 0;
}

