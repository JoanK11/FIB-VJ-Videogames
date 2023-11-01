#pragma once

#include"Sprite.h"
#include "Object.h"
class Tile
{
	protected:
		

		Sprite* spr;
		glm::vec2 pos;
		glm::vec2 originalPos;
	
public:
	Tile(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& shader,  Texture* text);
	Tile();
	virtual ~Tile();
	virtual void update(float dt);
	virtual void render();
	virtual bool isTouchable();
	virtual Object* actionToTouch(bool isSuperMario);
	virtual void restart();


};

