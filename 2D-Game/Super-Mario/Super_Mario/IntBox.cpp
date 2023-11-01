#include "IntBox.h"
#define HEIGHT 8
#define ANIMATION_TIME 250
IntBox::IntBox(glm::vec2 pos, glm::vec2 size, ShaderProgram* p, Object* obj) {
	Texture* text = new Texture();
	text->loadFromFile("images/interrogante.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spr = Sprite::createSprite(glm::ivec2(size.x, size.y), glm::vec2(0.25, 1.0), text, p);
	this->pos = pos;
	originalPos = pos;
	spr->setNumberAnimations(2);
	spr->setAnimationSpeed(0, 6);
	spr->addKeyframe(0, glm::vec2(0., 0.));
	spr->addKeyframe(0, glm::vec2(0.25, 0.));
	spr->addKeyframe(0, glm::vec2(0.50, 0.));
	spr->setAnimationSpeed(1, 20);
	spr->addKeyframe(1, glm::vec2(0.75, 0.));

	spr->setPosition(pos);
	spr->changeAnimation(0);

	this->obj = obj;
	currentState = IDLE;
	currentTime = 0;
}
void IntBox::update(float dt) {
	if (currentState == IDLE) {
		spr->update((int)dt);
	}
	else if (currentState == ANIMATION) {
		currentTime += dt;
		float y = -HEIGHT * sin(3.141592 / ANIMATION_TIME * currentTime);
		glm::vec2 p = pos;
		p.y += y;
		spr->setPosition(p);
		spr->update((int)dt);
		if (currentTime > ANIMATION_TIME) {
			currentTime = 0.;
			currentState = USED;
			spr->setPosition(pos);
			spr->changeAnimation(1);
		}
	}
}
void IntBox::render(){
	spr->render();
}
bool IntBox::isTouchable() {
	return true;
}

Object* IntBox::actionToTouch(bool isSuperMario) {
	if (currentState == IDLE) {
		currentState = ANIMATION;
		return obj;
	}
	return nullptr;
}

void IntBox::restart() {
	Tile::restart();
	currentState = IDLE;
	currentTime = 0;
	spr->setPosition(pos);
	spr->changeAnimation(0);
}