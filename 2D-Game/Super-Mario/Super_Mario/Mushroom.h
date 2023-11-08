#pragma once
#include "Object.h"
class Mushroom :
    public Object
{
private:
	enum MushroomState {
		WAITING,
		ANIMATION,
		VALID, 
		USED
	};
	float Vx, originalVx;
	float y_size;
	float currentTime;
	MushroomState currentState;
	ShaderProgram* program;
public:
	Mushroom(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& tileMapDisplay, TileMap* m, ShaderProgram* p, float Vx);
	virtual void actionOfObject(Player* ply);
	virtual void update(float dt);
	virtual void render(glm::vec2& cameraPos);
	virtual bool collide(const glm::vec2& plyPos, const glm::vec2& plySize);
	virtual void restart();
};

