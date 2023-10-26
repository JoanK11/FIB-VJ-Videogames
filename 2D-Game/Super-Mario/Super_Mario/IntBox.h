#pragma once
#include "Tile.h"
#include "Sprite.h"
class IntBox :
    public Tile
{
private:
    Sprite* spr;
public:
    IntBox(glm::vec2 pos, glm::vec2 size, ShaderProgram* p);
    virtual void update(float dt);
    void render();
};

