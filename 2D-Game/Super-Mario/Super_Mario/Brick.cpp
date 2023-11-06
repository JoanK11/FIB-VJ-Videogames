#include "Brick.h"
#define HEIGHT 10
#define ANIMATION_TIME 250
Brick::Brick(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& shader, Texture* text) :
	Tile(tilePosition, tileSize, texturePosition, textureSize, shader, text) {
	currentState = IDLE;
	currentTime = 0;
}
Brick::Brick(){}
Brick::~Brick() {}

void Brick::render(){
	if (currentState != DESTROYED) {
		Tile::render();
	}
}
bool Brick::isTouchable() {
	return  currentState == IDLE;
}

Object* Brick:: actionToTouch(bool isSuperMario) {
	if (isSuperMario) {
		currentState = DESTROYED;
		SoundManager::instance().playSFX("sfx/breakblock.wav");
	}
	else currentState = ANIMATION;
	return nullptr;
}

void Brick::update(float dt) {
	if (currentState == ANIMATION) {
		if (currentTime == 0) SoundManager::instance().playSFX("sfx/bump.wav");
		currentTime += dt;
		float y = -HEIGHT * sin(3.141592 / ANIMATION_TIME * currentTime);
		glm::vec2 p = pos;
		p.y += y;
		spr->setPosition(p);
		if (currentTime > ANIMATION_TIME) {
			currentTime = 0;
			currentState = IDLE;
			spr->setPosition(pos);
		}
	}
}

void Brick::restart() {
	Tile::restart();
	currentTime = 0;
	currentState = IDLE;
	spr->setPosition(pos);
}