#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include <iostream>
#include "Score.h"

class Enemy {

public:
	Enemy();
	~Enemy();

	virtual void update(int deltaTime, float xmin);
	void render();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2& pos);
	virtual int collision(const glm::vec2& pos, const glm::vec2& size);

	Sprite* sprite;
	Texture spritesheet;
	glm::ivec2 tileMapDispl, posEnemy;
	int dir;
	TileMap *map;
	bool bDelete;

private:
	
};

#endif _ENEMY_INCLUDE