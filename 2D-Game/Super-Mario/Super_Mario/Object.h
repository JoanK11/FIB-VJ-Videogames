#ifndef OBJECT_INCLUDE
#define OBJECT_INCLUDE

// forward declaration (.h) + circular inclusion (.cpp) for resolve the problem of circular dependency
class Player;
class TileMap;
#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"
#include "SoundManager.h"
#include "Score.h"
#include "Text.h"

#define TIME_TEXT 1000


class Object
{
protected:
	Sprite *spr;
	TileMap *map;
	glm::ivec2 pos, size;
	glm::ivec2 originalPos;
	glm::ivec2 tileMapDisplay;
	SoundManager sound;

	// Text
	Text text;
	glm::ivec2 textPos;
	float timeText;
	bool showingText;

public:
	Object(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& tileMapDisplay, TileMap *m);
	virtual ~Object() {};
	virtual void actionOfObject(Player *ply) = 0;
	virtual bool collide(const glm::vec2& plyPos, const glm::vec2& plySize);
	virtual void update(float dt) = 0;
	virtual void render(glm::vec2& cameraPos) = 0;
	virtual void restart();
	void showText();
};

#endif;


