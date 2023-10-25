#pragma once
#include "Object.h"
#include "ShaderProgram.h"
class Coin :
    public Object
{
protected:
    enum states {
        NOT_ACHIEVED,
        ANIMATION,
        DISAPPEARD,
    };
    float currentTime;
    states currentState;
public:
    Coin(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& tileMapDisplay, TileMap* m, ShaderProgram* p);
    virtual ~Coin() {};
    virtual void actionOfObject(Player* ply);
    virtual bool collide(const glm::vec2& plyPos, const glm::vec2& plySize);
    virtual void update(float dt);
    virtual void render();
};

