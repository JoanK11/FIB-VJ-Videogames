#pragma once
#include "Tile.h"
class Brick :
    public Tile
{
    private:
        bool isDestroyed;
    public:
        Brick(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& shader, Texture* text);
        Brick();
        virtual ~Brick();
        virtual void render();

        virtual bool isTouchable();

};

