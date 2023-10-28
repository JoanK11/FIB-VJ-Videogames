#pragma once
#include "Object.h"
class Star :
    public Object
{
private:
	enum StarState {
		ANIMATION,
		VALID,
		USED
	};
	float Vx;
	float y_size;
	float currentTime;
	StarState currentState;
	ShaderProgram* program;
	bool bJumping;
	int startY;
	int jumpAngle;
public:
	Star(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& tileMapDisplay, TileMap* m, ShaderProgram* p, float Vx);
	virtual void actionOfObject(Player* ply);
	virtual void update(float dt);
	virtual void render();
	virtual bool collide(const glm::vec2& plyPos, const glm::vec2& plySize);
};

