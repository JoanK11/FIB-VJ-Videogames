#include "Brick.h"
#define HEIGHT 10
#define ANIMATION_TIME 250
#define DESTROYED_ANIMATION_TIME 10000
#define X 10.0
#define Y 220.0

Brick::Brick(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& shader, Texture* text) :
	Tile(tilePosition, tileSize, texturePosition, textureSize, shader, text) {
	glm::vec2 miniQuadSize = tileSize / 2.f;
	glm::vec2 miniTextureSize = textureSize / 2.f;
	topLeft = Sprite::createSprite(miniQuadSize, miniTextureSize, text, &shader);
	topRight = Sprite::createSprite(miniQuadSize, miniTextureSize, text, &shader);
	bottomLeft = Sprite::createSprite(miniQuadSize, miniTextureSize, text, &shader);
	bottomRight = Sprite::createSprite(miniQuadSize, miniTextureSize, text, &shader);

	topLeft->setNumberAnimations(1);
	topRight->setNumberAnimations(1);
	bottomLeft->setNumberAnimations(1);
	bottomRight->setNumberAnimations(1);
	
	topLeft->setAnimationSpeed(0, 1);
	topRight->setAnimationSpeed(0, 1);
	bottomLeft->setAnimationSpeed(0, 1);
	bottomRight->setAnimationSpeed(0, 1);
	
	topLeft->addKeyframe(0, texturePosition + glm::vec2(0., 0.));
	topRight->addKeyframe(0, texturePosition + glm::vec2(miniTextureSize.x, 0.));
	bottomLeft->addKeyframe(0, texturePosition + glm::vec2(0., miniTextureSize.y));
	bottomRight->addKeyframe(0, texturePosition + miniTextureSize);
	
	topLeft->changeAnimation(0);
	topRight->changeAnimation(0);
	bottomLeft->changeAnimation(0);
	bottomRight->changeAnimation(0);

	posTL = tilePosition;
	posTR = tilePosition + glm::vec2(miniQuadSize.x, 0);
	posBL = tilePosition + glm::vec2(0, miniQuadSize.y);
	posBR = tilePosition + miniQuadSize;
	topLeft->setPosition(posTL);
	topRight->setPosition(posTR);
	bottomRight->setPosition(posBR);
	bottomLeft->setPosition(posBL);
	currentState = IDLE;
	currentTime = 0;
}
Brick::Brick(){}
Brick::~Brick() {}

void Brick::render(){
	if (currentState != DESTROYED) {
		if (currentState == ANIMATION_DESTROYED) {
			topLeft->render();
			topRight->render();
			bottomLeft->render();
			bottomRight->render();
			return;
		}
		Tile::render();
	}
}
bool Brick::isTouchable() {
	return  currentState == IDLE || currentState == ANIMATION;
}

Object* Brick:: actionToTouch(bool isSuperMario) {
	if (isSuperMario) {
		currentState = ANIMATION_DESTROYED;
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
	if (currentState == ANIMATION_DESTROYED) {
		currentTime += dt;
		float timeInSecond = currentTime / 1000.f;
		float posX = X * timeInSecond;
		float posY = Y * timeInSecond * timeInSecond;


		glm::vec2 nPosTL = posTL + glm::vec2(-2.f* posX , posY);
		glm::vec2 nPosTR = posTR + glm::vec2(2.f * posX, posY);
		glm::vec2 nPosBL = posBL + glm::vec2(-1.f * posX, posY);
		glm::vec2 nPosBR = posBR + glm::vec2(1.f * posX, posY);
		topLeft->setPosition(nPosTL);
		topRight->setPosition(nPosTR);
		bottomLeft->setPosition(nPosBL);
		bottomRight->setPosition(nPosBR);
		if (currentTime >= DESTROYED_ANIMATION_TIME) {
			currentState = DESTROYED;
			currentTime = 0;
		}

	}
}

void Brick::restart() {
	Tile::restart();
	currentTime = 0;
	currentState = IDLE;
	spr->setPosition(pos);
	topLeft->setPosition(posTL);
	topRight->setPosition(posTR);
	bottomRight->setPosition(posBR);
	bottomLeft->setPosition(posBL);
}

bool Brick::isMoving() {
	return currentState == ANIMATION;
}