#include "Menu.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Game.h"

enum PlayerAnims {
	MUSHROOM
};

void Menu::init(const glm::ivec2& tileMapPos, ShaderProgram& texProgram) {
	visible = true;
	pos = 0, posPlayer = glm::ivec2(0, 0);
	keyUpPressed = false, keyDownPressed = false;
	keyEnterPressed = false;

	spritesheet.loadFromFile("images/blocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(4, 4), glm::vec2(1, 1), &spritesheet, &texProgram);
	sprite->setNumberAnimations(1);

	// ----- IDLE -----
	sprite->setAnimationSpeed(MUSHROOM, 1);
	sprite->addKeyframe(MUSHROOM, glm::vec2(0.5, 0.5));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font!!!" << endl;
	}
}

bool Menu::showingMenu() const {
	return visible;
}

void Menu::update(int deltaTime) {
	sprite->update(deltaTime);
	sprite->changeAnimation(MUSHROOM);

	bool keyUp = Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey('w') || Game::instance().getKey('W');
	bool keyDown = Game::instance().getSpecialKey(GLUT_KEY_DOWN) || Game::instance().getKey('s') || Game::instance().getKey('S');
	bool keyEnter = Game::instance().getKey(13) || Game::instance().getKey(32);

	if (!keyUpPressed && keyUp) {
		keyUpPressed = true;
		if (pos == 0) pos = 1;
		else --pos;
	}
	if (!keyDownPressed && keyDown) {
		keyDownPressed = true;
		pos = (pos + 1) % 2;
	}

	if (!keyEnterPressed && keyEnter) {
		keyEnterPressed = true;
		//if (pos == 0) {
		visible = !visible;
		//}
	}

	if (!keyUp) keyUpPressed = false;
	if (!keyDown) keyDownPressed = false;
	if (!keyEnter) keyEnterPressed = false;

	//posPlayer.y = 10 + 16 * pos;
	glm::vec2 posSS = glm::vec2(150, 150);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Menu::render() {
	if (visible) {
		text.render("START GAME", glm::vec2(288, 288), 18, glm::vec4(1, 1, 1, 1));
		text.render("INSTRUCTIONS", glm::vec2(288, 366), 18, glm::vec4(1, 1, 1, 1));
		text.render("CREDITS", glm::vec2(288, 400), 18, glm::vec4(1, 1, 1, 1));
		sprite->render();
	}
}

void Menu::setPosition(const glm::vec2& pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(100), float(100)));
}
