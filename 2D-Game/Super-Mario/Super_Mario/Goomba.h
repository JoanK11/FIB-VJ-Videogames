#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Enemy.h"

class Goomba : public Enemy {
public:

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, float xmin, float xmax);

	int collision(const glm::vec2& pos, const glm::vec2& size);
	void restart();
	void kill();
	bool canCollide(float xmin, float xmax);
private:
	bool died;
	bool dying;
};

#endif _GOOMBA_INCLUDE
