#include "Flag.h"
#define HORIZONTAL_DISPLAY 18
#define TUBE_DISPLAY 10
#define MAX_PUNTUATION 1000.0
#define VELOCITY 2.
Flag::Flag(int ymin, int ymax, int xlim, const glm::vec2& tileMapDisplay, ShaderProgram* p) {
	this->ymin = ymin;
	this->ymax = ymax;
	this->xlim = xlim;
	yminOriginal = ymin;
	ymaxOriginal = ymax;
	xlimOriginal = xlim;
	this->tileMapDisplay = tileMapDisplay;
	currentState = IDLE;
	Texture* text = new Texture();
	text->loadFromFile("images/flag.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spr = Sprite::createSprite(glm::vec2(32, 32), glm::vec2(1., 1.), text, p);
	spr->setNumberAnimations(1);
	spr->setAnimationSpeed(0, 1);
	spr->addKeyframe(0, glm::vec2(0, 0));
	spr->changeAnimation(0);
	spr->setPosition(glm::vec2(xlim - HORIZONTAL_DISPLAY, ymin) + tileMapDisplay);
}
bool Flag::touchTheFlag(const glm::ivec2& pos, const glm::ivec2& size, bool superMario) {
	if (currentState == END) false;
	bool condition = pos.x + size.x >= xlim + TUBE_DISPLAY;
	if (currentState == IDLE && condition) {
		currentState = ANIMATION;
		double yPlayer = pos.y;
		if (superMario) yPlayer -= 32;
		double percentage = max(0., (yPlayer-ymin) / (ymax - ymin));
		percentage = 1 - percentage;
		int value_to_add = (int)(percentage * MAX_PUNTUATION);
		Score::instance().increaseScore(value_to_add);

		return true;
	}
	else if (currentState == ANIMATION && condition) return true;
	return false;

}
bool Flag::update(float dt) {
	if (currentState == IDLE || currentState == END) return false;
	if (currentState == ANIMATION) {
		ymin += VELOCITY;
		spr->setPosition(glm::vec2(xlim - HORIZONTAL_DISPLAY, ymin) + tileMapDisplay);
		if (ymin >= ymax) {
			ymin = ymax;
			spr->setPosition(glm::vec2(xlim - HORIZONTAL_DISPLAY, ymin) + tileMapDisplay);
			currentState = END;
			return  true;
		}
		return false;
	}
}
void Flag::render() {
	spr->render();
}
void Flag::restart() {
	this->ymin = yminOriginal;
	this->ymax = ymaxOriginal;
	this->xlim = xlimOriginal;
	currentState = IDLE;
	spr->setPosition(glm::vec2(xlim - HORIZONTAL_DISPLAY, ymin) + tileMapDisplay);
}