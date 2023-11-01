#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "TileMap.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 132
#define FALL_STEP 4


enum PlayerAnims {
	STAND_RIGHT, STAND_LEFT, MOVE_RIGHT, MOVE_LEFT, JUMP_RIGHT, 
	JUMP_LEFT, CHANGE_RIGHT, CHANGE_LEFT, DOWN_SUPER_RIGHT, DOWN_SUPER_LEFT, DEAD
};
Player::~Player() {}
void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	bJumping = false, bFalling  = false;

	// Mario Types
	superMario = false, superMarioKey = false;
	starMario = false,  starMarioKey  = false;
	starMario = false,  starMarioKey  = false;

	/* Key States */
	keyJumpPressed = false;

	// Velocity
	Vx = 0, Vy = 0;

	/* ----------------------- */
	/* -------- MARIO -------- */
	/* ----------------------- */

	spritesheet.loadFromFile("images/spriteMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(11);
		
		// ----- IDLE -----
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.125f));
		
		// ----- MOVING -----
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1250f, 0.f)); // MOVE_1
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1250f, 0.f)); // MOVE_1
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.f)); // MIDDLE
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.f)); // MOVE_2
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.f)); // MOVE_2
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.f)); // MIDDLE
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1250f, 0.125f)); // MOVE_1
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1250f, 0.125f)); // MOVE_1
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.125f)); // MIDDLE
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.125f)); // MOVE_2
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.125f)); // MOVE_2
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.125f)); // MIDDLE

		// ----- JUMPING -----
		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.3125f, 0.125f));

		// CHANGE_DIRECTION
		sprite->setAnimationSpeed(CHANGE_RIGHT, 8);
		sprite->addKeyframe(CHANGE_RIGHT, glm::vec2(0.2500f, 0.f));

		sprite->setAnimationSpeed(CHANGE_LEFT, 8);
		sprite->addKeyframe(CHANGE_LEFT, glm::vec2(0.2500f, 0.125f));

		// DOWN_SUPER
		sprite->setAnimationSpeed(DOWN_SUPER_RIGHT, 8);
		sprite->addKeyframe(DOWN_SUPER_RIGHT, glm::vec2(0.3750f, 0.375f));

		sprite->setAnimationSpeed(DOWN_SUPER_LEFT, 8);
		sprite->addKeyframe(DOWN_SUPER_LEFT, glm::vec2(0.3750f, 0.5f));

		// DEAD
		sprite->setAnimationSpeed(DEAD, 8);
		sprite->addKeyframe(DEAD, glm::vec2(0.3750f, 0.f));
		
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	/* ----------------------- */
	/* ----- SUPER MARIO ----- */
	/* ----------------------- */

	superSprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.0625, 0.25), &spritesheet, &shaderProgram);
	superSprite->setNumberAnimations(9);

		// ----- IDLE -----
		superSprite->setAnimationSpeed(STAND_RIGHT, 8);
		superSprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.25f));

		superSprite->setAnimationSpeed(STAND_LEFT, 8);
		superSprite->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.5f));

		// ----- MOVING -----
		superSprite->setAnimationSpeed(MOVE_RIGHT, 8);
		superSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.25f));
		superSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1250f, 0.25f));
		superSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.25f));
		superSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1250f, 0.25f));

		superSprite->setAnimationSpeed(MOVE_LEFT, 8);
		superSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.5f));
		superSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1250f, 0.5f));
		superSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.5f));
		superSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1250f, 0.5f));

		// ----- JUMPING -----
		superSprite->setAnimationSpeed(JUMP_RIGHT, 8);
		superSprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.25f));

		superSprite->setAnimationSpeed(JUMP_LEFT, 8);
		superSprite->addKeyframe(JUMP_LEFT, glm::vec2(0.3125f, 0.5f));

		// CHANGE_DIRECTION
		superSprite->setAnimationSpeed(CHANGE_RIGHT, 8);
		superSprite->addKeyframe(CHANGE_RIGHT, glm::vec2(0.2500f, 0.25f));

		superSprite->setAnimationSpeed(CHANGE_LEFT, 8);
		superSprite->addKeyframe(CHANGE_LEFT, glm::vec2(0.2500f, 0.5f));

		// DEAD
		superSprite->setAnimationSpeed(DEAD, 8);
		superSprite->addKeyframe(DEAD, glm::vec2(0.3750f, 0.25f));

	superSprite->changeAnimation(0);
	superSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime, float xmin, float& xmax) {

	/* --- Mario Sprites --- */
	Sprite* activeSprite = sprite;
	if (superMario)     activeSprite = superSprite;
	else if (starMario) activeSprite = starSprite;
	activeSprite->update(deltaTime);

	/* --- Movement Keys --- */
	bool keyLeft  = Game::instance().getSpecialKey(GLUT_KEY_LEFT) || Game::instance().getKey('a') || Game::instance().getKey('A');
	bool keyRight = Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || Game::instance().getKey('d') || Game::instance().getKey('D');
	bool keyUp    = Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(32) ||
		            Game::instance().getKey('w') || Game::instance().getKey('W');
	bool keyDown  = Game::instance().getSpecialKey(GLUT_KEY_DOWN) || Game::instance().getKey('s') || Game::instance().getKey('S');
	bool keyRun   = Game::instance().getModifierKey(0);

	/*
	if (keyLeft) cout << "LEFT" << endl;
	if (keyRight) cout << "RIGHT" << endl;
	if (keyUp) cout << "UP" << endl;
	if (keyDown) cout << "DOWN" << endl;
	*/

	/* --- Power-Up Keys --- */
	bool keySuperMario = Game::instance().getKey('m') || Game::instance().getKey('M');
	bool keyStarMario  = Game::instance().getKey('g') || Game::instance().getKey('G');

	/* Sprite Selection */
	if (!superMario && !superMarioKey && keySuperMario) {
		superMario = true, superMarioKey = true, starMario = false;
		superSprite->changeAnimation(activeSprite->animation());
	}
	else if (superMario && !superMarioKey && keySuperMario) {
		superMario = false, superMarioKey = true;
		activeSprite->changeAnimation(sprite->animation());
	}

	/* PHYSICS */
	glm::ivec2 dimMario = glm::ivec2(32, 32);
	if (superMario) dimMario = glm::ivec2(32, 64);

	/* MOVEMENT */
	if (keyLeft) {
		if (!bJumping && !bFalling && activeSprite->animation() != MOVE_LEFT) {
			//cout << "l1" << endl;
			activeSprite->changeAnimation(MOVE_LEFT);
		}
		bool colision = false;
		if (!bJumping && !bFalling && Vx > 0) {
			//cout << "    l2" << endl;
			activeSprite->changeAnimation(CHANGE_RIGHT);
			if (map->collisionMoveRight(posPlayer, dimMario, &posPlayer.x, superMario)) {
				//cout << "      l3" << endl;
				Vx = 0;
				activeSprite->changeAnimation(STAND_RIGHT);
				colision = true;
			}
		}

		if (keyRun) {
			if (Vx > -5) Vx -= 0.2f;
		}
		else {
			if (Vx < -3) Vx += 0.2f;
			else if (Vx > -3) Vx -= 0.2f;
		}

		if (map->collisionMoveLeft(posPlayer, dimMario, &posPlayer.x, superMario)) {
			//cout << "        l4" << endl;
			Vx = 0;
			activeSprite->changeAnimation(STAND_LEFT);
		}
		else if (!colision) {
			//cout << "          l5" << endl;
			posPlayer.x += Vx;
		}

		// Vx Correction
		if (Vx < -5) Vx = -5;
		else if (!keyRun && Vx < -3) Vx = -3;
	}

	else if (keyRight) {
		if (!bJumping && !bFalling && activeSprite->animation() != MOVE_RIGHT) {
			//cout << "r1" << endl;
			activeSprite->changeAnimation(MOVE_RIGHT);
		}
		bool colision = false;
		if (!bJumping && !bFalling && Vx < 0) {
			//cout << "    r2" << endl;
			activeSprite->changeAnimation(CHANGE_LEFT);
			if (map->collisionMoveLeft(posPlayer, dimMario, &posPlayer.x, superMario)) {
				//cout << "      r3" << endl;
				Vx = 0;
				activeSprite->changeAnimation(STAND_LEFT);
				colision = true;
			}
		}

		if (keyRun) {
			if (Vx < 5) Vx += 0.2f;
		}
		else {
			if (Vx > 3) Vx -= 0.2f;
			else if (Vx < 3) Vx += 0.2f;
		}

		if (map->collisionMoveRight(posPlayer, dimMario, &posPlayer.x, superMario)) {
			//cout << "        r4" << endl;
			Vx = 0;
			activeSprite->changeAnimation(STAND_RIGHT);
		}
		else if (!colision) {
			//cout << "          r5" << endl;
			posPlayer.x += Vx;
		}

		// Vx Correction
		if (Vx > 5) Vx = 5;
		else if (!keyRun && Vx > 3) Vx = 3;
	}

	else {
		if (Vx > 0.1f) {
			Vx -= 0.1f;
			posPlayer.x += Vx;
		}
		else if (Vx < -0.1f) {
			Vx += 0.1f;
			posPlayer.x += Vx;
		}
		else {
			Vx = 0;
			if (activeSprite->animation() == CHANGE_LEFT || activeSprite->animation() == MOVE_LEFT)
				activeSprite->changeAnimation(STAND_LEFT);
			else if (activeSprite->animation() == CHANGE_RIGHT || activeSprite->animation() == MOVE_RIGHT)
				activeSprite->changeAnimation(STAND_RIGHT);
		}

		if (map->collisionMoveRight(posPlayer, dimMario, &posPlayer.x, superMario)) Vx = 0;
		else if (map->collisionMoveLeft(posPlayer, dimMario, &posPlayer.x, superMario)) Vx = 0;
	}

	if (keyDown) {

	}

	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180) {
			bJumping = false;
			posPlayer.y = startY;
		}
		else {
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

			if (jumpAngle > 90) { // is falling down
				bFalling = true;
				bJumping = false;//!map->collisionMoveDown(posPlayer, dimMario, &posPlayer.y);
				//if (!bJumping) posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			}

			else bJumping = !map->collisionMoveUp(posPlayer, dimMario, &posPlayer.y, superMario);// && (!keyJumpPressed;

		}

		if (activeSprite->animation() == MOVE_LEFT || activeSprite->animation() == STAND_LEFT ||
			activeSprite->animation() == CHANGE_LEFT)
			activeSprite->changeAnimation(JUMP_LEFT);
		else if (activeSprite->animation() == MOVE_RIGHT || activeSprite->animation() == STAND_RIGHT ||
			activeSprite->animation() == CHANGE_RIGHT)
			activeSprite->changeAnimation(JUMP_RIGHT);
	}
	else {
		posPlayer.y += FALL_STEP;

		if (!bFalling && (activeSprite->animation() == JUMP_LEFT || activeSprite->animation() == JUMP_RIGHT)) {
			if (Vx > 0) activeSprite->changeAnimation(STAND_RIGHT);
			else if (Vx < 0) activeSprite->changeAnimation(STAND_LEFT);
			else {
				if (activeSprite->animation() == JUMP_LEFT)
					activeSprite->changeAnimation(STAND_LEFT);
				else
					activeSprite->changeAnimation(STAND_RIGHT);
			}
		}

		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
			bFalling = false;
			if (!keyJumpPressed && keyUp) {
				if (superMario)	sound.playSFX("sfx/jump-super.wav");
				else sound.playSFX("sfx/jump-small.wav");
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
		else {
			bFalling = true;
			if (activeSprite->animation() == MOVE_LEFT || activeSprite->animation() == STAND_LEFT)
				activeSprite->changeAnimation(JUMP_LEFT);
			else if (activeSprite->animation() == MOVE_RIGHT || activeSprite->animation() == STAND_RIGHT)
				activeSprite->changeAnimation(JUMP_RIGHT);
		}
	}

	/* Screen Scroll */
	if (posPlayer.x + 32 + tileMapDispl.x > xmax) {
		xmax = posPlayer.x + 32 + tileMapDispl.x;
	}
	else if (posPlayer.x < xmin && !keyRight) {
		posPlayer.x = xmin;
		Vx = 0;
	}

	//cout << jumpAngle << endl;
	/*
	if (bJumping && bFalling) cout << "bJumping && bFalling" << endl;
	else if (bJumping) cout << "bJumping" << endl;
	else if (bFalling) cout << "bFalling" << endl;
	else cout << "." << endl;
	*/

	/* Key Updates */
	keyJumpPressed = keyUp;
	superMarioKey = keySuperMario;

	/* --- Sprite Update --- */
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	superSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
	//te he comentado abajo porque sino te explota el programa
	//starSprite-> setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
}

void Player::render() {
	//cout << "Pos.x: " << posPlayer.x << " / Pos.y: " << posPlayer.y << endl;
	if (superMario) superSprite->render();
	else sprite->render();
}

void Player::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


void Player::setSuperMario(bool b) {
	superMario = b;
}


glm::vec2 Player::getPos() const {
	glm::vec2 pos = posPlayer;
	if (superMario) {
		pos.y -= 32;
		return pos;
	}
	return pos;
}
glm::vec2 Player::getSize() const {
	if (superMario) return glm::vec2(32, 64);
	return glm::vec2(32, 32);
}


void Player::setStarMario(bool b){
	starMario = b;
	starMarioTime = 3000;
}