#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE
// forward declaration (.h) + circular inclusion (.cpp) for resolve the problem of circular dependency
class TileMap;
#include "Sprite.h"
#include "SoundManager.h"



// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player {

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, float xmin, float& xmax);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void setSuperMario(bool b);
	void setStarMario(bool b);


	glm::vec2 getPos() const;
	glm::vec2 getSize() const;
private:
	SoundManager sound;

	bool bJumping, bFalling;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite, *superSprite, *starSprite;
	TileMap *map;
	float Vx, Vy;

	bool keyJumpPressed;

	// Mario Types
	bool superMario, superMarioKey;
	bool starMario, starMarioKey;
	int starMarioTime;
};


#endif // _PLAYER_INCLUDE


