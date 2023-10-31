#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Enemy.h"

class Goomba : public Enemy {
public:

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, float xmin, float xmax);

	int collision(const glm::vec2& pos, const glm::vec2& size);

private:
	bool died;
};

#endif _GOOMBA_INCLUDE
