#ifndef OBJECT_INCLUDE
#define OBJECT_INCLUDE
class Player;
class TileMap;
#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"


class Object
{
protected:
	Sprite *spr;
	TileMap *map;
	glm::ivec2 pos, size;
	glm::ivec2 tileMapDisplay;
public:
	Object(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& tileMapDisplay, TileMap *m);
	virtual ~Object() {};
	virtual void actionOfObject(Player *ply) = 0;
	virtual bool collide(const glm::vec2& plyPos, const glm::vec2& plySize);
	virtual void update(float dt) = 0;
	virtual void render() = 0;
};
#include "Player.h"
#endif;


