#ifndef _KOOPA_INCLUDE
#define _KOOPA_INCLUDE

#include "Enemy.h"

class Koopa : public Enemy {
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime, float xmin, float xmax);

    int collision(const glm::vec2& pos, const glm::vec2& size);
    virtual void restart();
    
    glm::ivec2 getSize() const;
    bool canKillEnemies() const;
    void kill();

private:
    enum KoopaStates {
        NOT_SPAWNED, MOVING, SHELL_IDLE,
        SHELL_MOVING, DIED, DYING
    };
    KoopaStates state;
};

#endif // _KOOPA_INCLUDE