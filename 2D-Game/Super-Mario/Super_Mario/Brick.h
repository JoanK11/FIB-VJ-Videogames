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
            ANIMATION_DESTROYED,
            DESTROYED
        };
        BrickState currentState;
        float currentTime;
        Sprite* topLeft;
        Sprite* topRight;
        Sprite* bottomLeft;
        Sprite* bottomRight;
        glm::vec2 posTL, posTR, posBL, posBR;
    public:
        Brick(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& shader, Texture* text);
        Brick();
        virtual ~Brick();
        virtual void render();
        virtual bool isTouchable();
        virtual Object* actionToTouch(bool isSuperMario);
        virtual void update(float dt);
        virtual void restart();
        virtual bool isMoving();
};

