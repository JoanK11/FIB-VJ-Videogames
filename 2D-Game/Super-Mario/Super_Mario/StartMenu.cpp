#include "StartMenu.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Game.h"

enum PlayerAnims {
	MUSHROOM
};

void StartMenu::init() {
	initShaders();

	visible = true;
	pos = 0, posPlayer = glm::ivec2(0, 0);
	keyUpPressed = false, keyDownPressed = false;
	keyEnterPressed = false;

	spritesheet.loadFromFile("images/spriteMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(2, 2), glm::vec2(0.0625, 0.125), &spritesheet, &texProgram);
	sprite->setNumberAnimations(1);

	// ----- IDLE -----
	sprite->setAnimationSpeed(MUSHROOM, 8);
	sprite->addKeyframe(MUSHROOM, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(100), float(100)));

	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font!!!" << endl;
	}
}

bool StartMenu::showingMenu() const {
	return visible;
}

void StartMenu::update(int deltaTime) {
	sprite->update(deltaTime);
	sprite->changeAnimation(MUSHROOM);

	bool keyUp    = Game::instance().getSpecialKey(GLUT_KEY_UP)     || Game::instance().getKey('w') || Game::instance().getKey('W');
	bool keyDown  = Game::instance().getSpecialKey(GLUT_KEY_DOWN)   || Game::instance().getKey('s') || Game::instance().getKey('S');
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
		if (pos == 0) {
			visible = !visible;
		}
	}

	if (!keyUp) keyUpPressed = false;
	if (!keyDown) keyDownPressed = false;
	if (!keyEnter) keyEnterPressed = false;

	//posPlayer.y = 10 + 16 * pos;
	sprite->setPosition(glm::vec2(float(150), float(150)));
	
	
}

void StartMenu::render() {
	sprite->setPosition(glm::vec2(float(1000), float(1000)));
	
	if (visible) {
		text.render("START GAME", glm::vec2(288, 288), 18, glm::vec4(1, 1, 1, 1));
		text.render("INSTRUCTIONS", glm::vec2(288, 366), 18, glm::vec4(1, 1, 1, 1));
		text.render("CREDITS", glm::vec2(288, 400), 18, glm::vec4(1, 1, 1, 1));
		sprite->render();
	}
}

void StartMenu::setPosition(const glm::vec2& pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(100), float(100)));
}

void StartMenu::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled()) {
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled()) {
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}