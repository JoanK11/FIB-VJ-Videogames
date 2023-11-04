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
	virtual ~Player();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, float xmin, float& xmax, SoundManager& soundScene);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void setSuperMario();
	void setStarMario();

	// Enemy Collisions
	void jumpEnemy();
	void collisionEnemy();

	// Check Player Conditions
	bool isDead();
	bool isImmune();

	void powerDown();
	bool isStarMario() const;


	glm::vec2 getPos() const;
	glm::vec2 getSize() const;

private:
	SoundManager sound;
	Texture spritesheet, starSpritesheet, superStarSpritesheet;
	Sprite *sprite, *superSprite, *starSprite, *superStarSprite;
	TileMap *map;

	bool bJumping, bFalling;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	
	
	float Vx, Vy;
	bool jumpingEnemy;

	bool keyJumpPressed;

	// Mario Types
	enum MarioStates { NORMAL, SUPER, STAR, DYING };
	MarioStates state;
	bool superMario, superMarioKey;

	/* Star Mario */
	bool starMario, starMarioKey;
	int starMarioTime;

	/* Power-Up Animation */
	float timePowerUpAnimation;
	bool bPowerUpAnimation;
	int renderPowerUpAnimation;

	/* Power-Down (Immunity) */
	float timeImmunity;
	bool bImmunity;
	int renderImmunity;

	int jumpDuration;
};


#endif // _PLAYER_INCLUDE


