#include "StartMenu.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Game.h"
#include <GL/glut.h>

enum PlayerAnims {
	MUSHROOM
};

void StartMenu::init() {
	initShaders();

	visible = true, state = 0;
	pos = 0;
	posYini = 212, espacio = 48;
	posPlayer = glm::ivec2(240, posYini - 24);

	/* Keys State */
	keyUpPressed = false, keyDownPressed = false;
	keyEnterPressed = false, keyEscPressed = false;

	/* MUSHROOM CHOOSER */
	spritesheet.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.03125f, 0.125f), &spritesheet, &texProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(MUSHROOM, 1);
	sprite->addKeyframe(MUSHROOM, glm::vec2(0.5f, 0.f));
	sprite->changeAnimation(0);
	sprite->setPosition(posPlayer);

	/* WALL */
	spriteWall = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH-128, SCREEN_HEIGHT-128), glm::vec2(0.5f, 1), &spritesheet, &texProgram);
	spriteWall->setNumberAnimations(1);
	spriteWall->setAnimationSpeed(MUSHROOM, 1);
	spriteWall->addKeyframe(MUSHROOM, glm::vec2(0.f, 0.f));
	spriteWall->changeAnimation(0);
	spriteWall->setPosition(glm::vec2(64, 64));

	spritesheet2.loadFromFile("images/cabrevj.png", TEXTURE_PIXEL_FORMAT_RGBA); // bertus.png
	sprite3 = Sprite::createSprite(glm::ivec2(128, 128), glm::vec2(1, 1), &spritesheet2, &texProgram);
	sprite3->setNumberAnimations(1);
	sprite3->setAnimationSpeed(MUSHROOM, 1);
	sprite3->addKeyframe(MUSHROOM, glm::vec2(0.f, 0.f));
	sprite3->changeAnimation(0);
	sprite3->setPosition(glm::vec2(346, 280));

	/* TEXT */
	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font super-mario-bros-nes!!!" << endl;
	}

	if (!textKeyboard.init("fonts/212keyboard.otf")) {
		cout << "Could not load font 212keyboard!!!" << endl;
	}
}

bool StartMenu::showingMenu() const {
	return visible;
}

void StartMenu::openMenu() {
	visible = true;
}

void StartMenu::update(int deltaTime) {
	if (!visible) return;

	sprite->update(deltaTime);
	sprite->changeAnimation(MUSHROOM);

	bool keyUp    = Game::instance().getSpecialKey(GLUT_KEY_UP)     || Game::instance().getKey('w') || Game::instance().getKey('W');
	bool keyDown  = Game::instance().getSpecialKey(GLUT_KEY_DOWN)   || Game::instance().getKey('s') || Game::instance().getKey('S');
	bool keyEnter = Game::instance().getKey(13) || Game::instance().getKey(32);
	bool keyEsc = Game::instance().getKey(27);

	if (!keyUpPressed && keyUp) {
		keyUpPressed = true;
		if (pos == 0) pos = 3;
		else --pos;
	}
	if (!keyDownPressed && keyDown) {
		keyDownPressed = true;
		pos = (pos + 1) % 4;
	}

	if (!keyEnterPressed && keyEnter) {
		keyEnterPressed = true;
		if (pos == 0) {
			visible = !visible;
			state = 0;
			sound.playSFX("sfx/here_we_go.wav");
		}
		else if (pos == 1) {
			if (state != 1) sound.playSFX("sfx/its_me_mario.wav");
			state = 1;
		}
		else if (pos == 2) {
			if (state != 2) sound.playSFX("sfx/mamma_mia.wav");
			state = 2;
			
		}
		else if (pos == 3) {
			exit(0);
		}
	}

	if (!keyEscPressed && keyEsc) {
		state = 0;
	}


	/* Update Key States */
	if (!keyUp)    keyUpPressed    = false;
	if (!keyDown)  keyDownPressed  = false;
	if (!keyEnter) keyEnterPressed = false;
	if (!keyEsc)   keyEscPressed   = false;

	posPlayer.y = posYini - 24 + espacio * pos;
	sprite->setPosition(posPlayer);
}

void StartMenu::render() {
	if (visible) {
		if (state == 0) {
			/* TEXT */
			text.render("START GAME", glm::vec2(288, posYini), 18, glm::vec4(1, 1, 1, 1));
			text.render("INSTRUCTIONS", glm::vec2(288, posYini + espacio), 18, glm::vec4(1, 1, 1, 1));
			text.render("CREDITS", glm::vec2(288, posYini + 2 * espacio), 18, glm::vec4(1, 1, 1, 1));
			text.render("EXIT", glm::vec2(288, posYini + 3 * espacio), 18, glm::vec4(1, 1, 1, 1));

			/* MUSHROOM CHOOSER */
			restartShaders();
			sprite->render();
		}
		else if (state == 1) {
			restartShaders();
			spriteWall->render();
			text.render("INSTRUCTIONS", glm::vec2(SCREEN_WIDTH / 2 - 216, 112), 32, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));

			/* MOVEMENT */
			text.render("Movement:",   glm::vec2(96, 176),  16, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
			textKeyboard.render("W",   glm::vec2(156, 218), 32, glm::vec4(1, 1, 1, 1));
			textKeyboard.render("ASD", glm::vec2(120, 254), 32, glm::vec4(1, 1, 1, 1));
			textKeyboard.render("g",   glm::vec2(120, 302), 32, glm::vec4(1, 1, 1, 1));
			text.render("Run",         glm::vec2(196, 292), 12, glm::vec4(1, 1, 1, 1));
			textKeyboard.render("Z",   glm::vec2(120, 350), 32, glm::vec4(1, 1, 1, 1));
			text.render("Mute",        glm::vec2(160, 340), 12, glm::vec4(1, 1, 1, 1));

			/* CHEATS */
			text.render("Cheats:",     glm::vec2(528, 176), 16, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
			textKeyboard.render("M",   glm::vec2(544, 228), 32, glm::vec4(1, 1, 1, 1));
			text.render("Super Mario", glm::vec2(584, 218), 12, glm::vec4(1, 1, 1, 1));
			textKeyboard.render("G",   glm::vec2(544, 276), 32, glm::vec4(1, 1, 1, 1));
			text.render("Star Mario",  glm::vec2(584, 266), 12, glm::vec4(1, 1, 1, 1));
			textKeyboard.render("12",  glm::vec2(544, 324), 32, glm::vec4(1, 1, 1, 1));
			text.render("Level",       glm::vec2(620, 314), 12, glm::vec4(1, 1, 1, 1));
			textKeyboard.render("a",   glm::vec2(544, 372), 32, glm::vec4(1, 1, 1, 1));
			text.render("Pause",       glm::vec2(600, 362), 12, glm::vec4(1, 1, 1, 1));
			text.render("Esc: Exit", glm::vec2(104, 400), 12, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
			// Esc = "m"
		}
		else if (state == 2) {
			restartShaders();
			spriteWall->render();
			sprite3->render();
			text.render("CREDITS", glm::vec2(SCREEN_WIDTH / 2 - 128, 112), 32, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
			text.render("Autors:", glm::vec2(88, 176), 16, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
			text.render("* Joan Caballero", glm::vec2(104, 208), 16, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
			text.render("* Jeremy Comino", glm::vec2(104, 232), 16, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
			text.render("Siempre te recordaremos D.E.P.", glm::vec2(164, 264), 16, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
			//text.render("Gracias por ser mi razon de existir", glm::vec2(108, 264), 16, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));

			//textKeyboard.render('E', glm::vec2(104, 404), 32, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
			text.render("Esc: Exit", glm::vec2(104, 400), 12, glm::vec4(0.98823f, 0.76078f, 0.73725f, 1.f));
		}
		
	}
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

void StartMenu::restartShaders() {
	texProgram.use();
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	texProgram.setUniformMatrix4f("viewMatrix", modelview);
	glm::mat4 projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
}