#include "StartMenu.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Game.h"

enum PlayerAnims {
	MUSHROOM
};

void StartMenu::init(const glm::ivec2& tileMapPos) {
	initShaders();

	visible = true;
	pos = 0, posPlayer = glm::ivec2(240, 260);
	keyUpPressed = false, keyDownPressed = false;
	keyEnterPressed = false;

	spritesheet.loadFromFile("images/spriteMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625f, 0.25f), &spritesheet, &texProgram);
	sprite->setNumberAnimations(1);

	// ----- IDLE -----
	sprite->setAnimationSpeed(MUSHROOM, 1);
	sprite->addKeyframe(MUSHROOM, glm::vec2(0.f, 0.25f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite->setPosition(glm::vec2(10, 10));
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
		if (pos == 0) pos = 2;
		else --pos;
	}
	if (!keyDownPressed && keyDown) {
		keyDownPressed = true;
		pos = (pos + 1) % 3;
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

	posPlayer.y = 260 + 48 * pos;
	sprite->setPosition(posPlayer);
}

void StartMenu::render() {
	if (visible) {
		text.Text::render("START GAME", glm::vec2(288, 288), 18, glm::vec4(1, 1, 1, 1));
		text.render("INSTRUCTIONS", glm::vec2(288, 336), 18, glm::vec4(1, 1, 1, 1));
		text.render("CREDITS", glm::vec2(288, 384), 18, glm::vec4(1, 1, 1, 1));
		texProgram.use();
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		glm::mat4 modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		glm::mat4 projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
		texProgram.setUniformMatrix4f("projection", projection);
		sprite->Sprite::render();
	}
}

void StartMenu::setPosition(const glm::vec2& pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(-20.f), float(0)));
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