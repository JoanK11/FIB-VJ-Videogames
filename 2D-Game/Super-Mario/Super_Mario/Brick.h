#pragma once
#include "Tile.h"
#include "SoundManager.h"

class Brick :
    public Tile
{
    private:
        enum BrickState {
            IDLE,
            ANIMATION,
            DESTROYED
        };
        BrickState currentState;
        float currentTime;

    public:
        Brick(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& shader, Texture* text);
        Brick();
        virtual ~Brick();
        virtual void render();
        virtual bool isTouchable();
        virtual Object* actionToTouch(bool isSuperMario);
        virtual void update(float dt);
        virtual void restart();

};

