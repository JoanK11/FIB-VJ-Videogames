#pragma once
#include "Tile.h"
#include "Sprite.h"

class IntBox :
    public Tile
{
private:
    enum IntBoxState {
        IDLE,
        ANIMATION,
        USED
    };
    Object* obj;
    IntBoxState currentState;
    float currentTime;
public:
    IntBox(glm::vec2 pos, glm::vec2 size, ShaderProgram* p);
    virtual void update(float dt);
    virtual void render();
    virtual bool isTouchable();
    virtual Object* actionToTouch(bool isSuperMario);
};

