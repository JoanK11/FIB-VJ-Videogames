#include "Flag.h"
#include <iostream>

#define HORIZONTAL_DISPLAY 18
#define TUBE_DISPLAY 10
#define MAX_PUNTUATION 5000.0
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
	Texture* texture = new Texture();
	texture->loadFromFile("images/flag.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spr = Sprite::createSprite(glm::vec2(32, 32), glm::vec2(1., 1.), texture, p);
	spr->setNumberAnimations(1);
	spr->setAnimationSpeed(0, 1);
	spr->addKeyframe(0, glm::vec2(0, 0));
	spr->changeAnimation(0);
	spr->setPosition(glm::vec2(xlim - HORIZONTAL_DISPLAY, ymin) + tileMapDisplay);
	showingText = false;

	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font!!!" << endl;
	}
}

bool Flag::touchTheFlag(const glm::ivec2& pos, const glm::ivec2& size, bool superMario) {
	
	bool condition = pos.x + size.x >= xlim + TUBE_DISPLAY;
	if (!condition) return false;
	if (currentState == IDLE && condition) {
		currentState = ANIMATION;
		double yPlayer = pos.y;
		if (superMario) yPlayer -= 32;
		double percentage = max(0., (yPlayer-ymin) / (ymax - ymin));
		percentage = 1 - percentage;
		score = (int)(percentage * MAX_PUNTUATION);
		showText(glm::vec2(pos.x, yPlayer));
		Score::instance().increaseScore(score);

		return true;
	}
	return true;

}
bool Flag::update(float dt) {
	if (showingText) timeText += dt;
	if (currentState == END) return true;
	if (currentState == IDLE) return false;
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
void Flag::render(glm::vec2& cameraPos) {
	spr->render();

	if (showingText) {
		float y = timeText / TIME_TEXT * 64;
		if (timeText > TIME_TEXT) showingText = false, timeText = 0;
		text.render(to_string(score), glm::vec2(textPos.x - cameraPos.x, textPos.y + tileMapDisplay.y - y), 14, glm::vec4(1, 1, 1, 1));
	}
}
void Flag::restart() {
	this->ymin = yminOriginal;
	this->ymax = ymaxOriginal;
	this->xlim = xlimOriginal;
	currentState = IDLE;
	spr->setPosition(glm::vec2(xlim - HORIZONTAL_DISPLAY, ymin) + tileMapDisplay);
}

void Flag::showText(glm::vec2& posPlayer) {
	showingText = true;
	timeText = 0;
	textPos = posPlayer;
}