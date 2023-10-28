#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include <iostream>
#include "Score.h"

#define VX 1
#define SPAWN_DISTANCE 18 // 13 (tiles visibles) + 5 (tiles not visible)
#define FALL_STEP 4

class Enemy {

public:
	Enemy();
	~Enemy();

	virtual void update(int deltaTime, float xmin, float xmax);
	void render();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2& pos);
	virtual int collision(const glm::vec2& pos, const glm::vec2& size);

	SoundManager sound;
	Sprite* sprite;
	Texture spritesheet;
	glm::ivec2 tileMapDispl, posEnemy;
	int dir;
	TileMap *map;
	bool bDelete, spawned;
	float currentTime;

private:
	
};

#endif _ENEMY_INCLUDE