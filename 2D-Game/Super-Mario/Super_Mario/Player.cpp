#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "TileMap.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_ANGLE_STEP_ENEMY 8
#define JUMP_HEIGHT 132
#define JUMP_HEIGHT_ENEMY 32
#define JUMP_HEIGHT_DYING 64
#define FALL_STEP 4

#define TIME_IMMUNE 4000
#define TIME_POWER_UP_ANIMATION 1200
#define TIME_STAR_MARIO 9000


enum PlayerAnims {
	STAND_RIGHT, STAND_LEFT, MOVE_RIGHT, MOVE_LEFT, JUMP_RIGHT, 
	JUMP_LEFT, CHANGE_RIGHT, CHANGE_LEFT, DEAD,
	CROUCHED_RIGHT, CROUCHED_LEFT, FLAG_LEFT, FLAG_RIGHT
};

Player::~Player() {
	if (sprite != NULL) delete sprite;
	if (superSprite != NULL) delete superSprite;
	if (starSprite != NULL) delete starSprite;
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, SoundManager *soundScene) {
	bJumping = false, bFalling = false; jumpingEnemy = false;
	auxFirstTime = true;
	// Mario Types
	superMario = false, superMarioKey = false;
	starMario = false,  starMarioKey  = false;
	
	state = NORMAL;

	/* Key States */
	keyJumpPressed = false;

	// Velocity
	Vx = 0, Vy = 0;
	gravity = 0;

	/* Sound */
	sound = soundScene;

	/* ----------------------- */
	/* -------- MARIO -------- */
	/* ----------------------- */

	spritesheet.loadFromFile("images/spriteMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(13);
		
		// ----- IDLE -----
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.125f));
		
		// ----- MOVING -----
		sprite->setAnimationSpeed(MOVE_RIGHT, 12);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1250f, 0.f)); // MOVE_1
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1250f, 0.f)); // MOVE_1
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.f)); // MIDDLE
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.f)); // MOVE_2
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.f)); // MOVE_2
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.f)); // MIDDLE
		
		sprite->setAnimationSpeed(MOVE_LEFT, 12);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1250f, 0.125f)); // MOVE_1
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1250f, 0.125f)); // MOVE_1
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.125f)); // MIDDLE
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.125f)); // MOVE_2
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.125f)); // MOVE_2
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.125f)); // MIDDLE

		// ----- JUMPING -----
		sprite->setAnimationSpeed(JUMP_RIGHT, 1);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 1);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.3125f, 0.125f));

		// CHANGE_DIRECTION
		sprite->setAnimationSpeed(CHANGE_RIGHT, 1);
		sprite->addKeyframe(CHANGE_RIGHT, glm::vec2(0.2500f, 0.f));

		sprite->setAnimationSpeed(CHANGE_LEFT, 1);
		sprite->addKeyframe(CHANGE_LEFT, glm::vec2(0.2500f, 0.125f));

		// DEAD
		sprite->setAnimationSpeed(DEAD, 1);
		sprite->addKeyframe(DEAD, glm::vec2(0.3750f, 0.f));

		// FLAG
		sprite->setAnimationSpeed(FLAG_RIGHT, 2);
		sprite->addKeyframe(FLAG_RIGHT, glm::vec2(0.4375f, 0.f));
		sprite->addKeyframe(FLAG_RIGHT, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(FLAG_LEFT, 2);
		sprite->addKeyframe(FLAG_LEFT, glm::vec2(0.4375f, 0.125f));
		sprite->addKeyframe(FLAG_LEFT, glm::vec2(0.5f, 0.125f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	/* ----------------------- */
	/* ----- SUPER MARIO ----- */
	/* ----------------------- */

	superSprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.0625, 0.25), &spritesheet, &shaderProgram);
	superSprite->setNumberAnimations(13);

		// ----- IDLE -----
		superSprite->setAnimationSpeed(STAND_RIGHT, 1);
		superSprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.25f));

		superSprite->setAnimationSpeed(STAND_LEFT, 1);
		superSprite->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.5f));

		// ----- MOVING -----
		superSprite->setAnimationSpeed(MOVE_RIGHT, 12);
		superSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.25f));
		superSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1250f, 0.25f));
		superSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.25f));
		superSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1250f, 0.25f));

		superSprite->setAnimationSpeed(MOVE_LEFT, 12);
		superSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.5f));
		superSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1250f, 0.5f));
		superSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.5f));
		superSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1250f, 0.5f));

		// ----- JUMPING -----
		superSprite->setAnimationSpeed(JUMP_RIGHT, 1);
		superSprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.25f));

		superSprite->setAnimationSpeed(JUMP_LEFT, 1);
		superSprite->addKeyframe(JUMP_LEFT, glm::vec2(0.3125f, 0.5f));

		// CHANGE_DIRECTION
		superSprite->setAnimationSpeed(CHANGE_RIGHT, 1);
		superSprite->addKeyframe(CHANGE_RIGHT, glm::vec2(0.2500f, 0.25f));

		superSprite->setAnimationSpeed(CHANGE_LEFT, 1);
		superSprite->addKeyframe(CHANGE_LEFT, glm::vec2(0.2500f, 0.5f));

		// DEAD (No existe)
		superSprite->setAnimationSpeed(DEAD, 1);
		superSprite->addKeyframe(DEAD, glm::vec2(0.3750f, 0.25f));

		// CROUCHED
		superSprite->setAnimationSpeed(CROUCHED_RIGHT, 1);
		superSprite->addKeyframe(CROUCHED_RIGHT, glm::vec2(0.3750f, 0.25f));

		superSprite->setAnimationSpeed(CROUCHED_LEFT, 1);
		superSprite->addKeyframe(CROUCHED_LEFT, glm::vec2(0.3750f, 0.5f));

		// FLAG
		superSprite->setAnimationSpeed(FLAG_RIGHT, 2);
		superSprite->addKeyframe(FLAG_RIGHT, glm::vec2(0.4375f, 0.25f));
		superSprite->addKeyframe(FLAG_RIGHT, glm::vec2(0.5f, 0.5f));

		superSprite->setAnimationSpeed(FLAG_LEFT, 2);
		superSprite->addKeyframe(FLAG_LEFT, glm::vec2(0.4375f, 0.25f));
		superSprite->addKeyframe(FLAG_LEFT, glm::vec2(0.5f, 0.5f));

	superSprite->changeAnimation(0);
	superSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));

	/* ----------------------- */
	/* ----- STAR MARIO ----- */
	/* ----------------------- */

	starSpritesheet.loadFromFile("images/spriteStarMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	starSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625f, 0.125f), &starSpritesheet, &shaderProgram);
	starSprite->setNumberAnimations(13);

	/* Idle */
	starSprite->setAnimationSpeed(STAND_RIGHT, 4);
	for (int i = 0; i < 4; ++i)	starSprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.125f * i));
	starSprite->setAnimationSpeed(STAND_LEFT, 4);
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.5f + 0.125f*i));

	/* Moving */
	starSprite->setAnimationSpeed(MOVE_RIGHT, 6);
	for (int i = 0; i < 12; ++i) {
		int i_x = i % 3;
		int i_y = (i % 4);
		starSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f + 0.0625f * i_x, 0.125f * i_y));
	}
	starSprite->setAnimationSpeed(MOVE_LEFT, 6);
	for (int i = 0; i < 12; ++i) {
		int i_x = i % 3;
		int i_y = (i % 4);
		starSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f + 0.0625f * i_x, 0.5f + 0.125f * i_y));
	}

	/* Jumping */
	starSprite->setAnimationSpeed(JUMP_RIGHT, 4);
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.125f * i));
	starSprite->setAnimationSpeed(JUMP_LEFT, 4);
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(JUMP_LEFT, glm::vec2(0.3125f, 0.5f + 0.125f * i));

	/* Change Direction */
	starSprite->setAnimationSpeed(CHANGE_RIGHT, 4);
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(CHANGE_RIGHT, glm::vec2(0.25f, 0.125f * i));
	starSprite->setAnimationSpeed(CHANGE_LEFT, 4);
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(CHANGE_LEFT, glm::vec2(0.25f, 0.5f + 0.125f * i));

	/* Dead */
	starSprite->setAnimationSpeed(DEAD, 4);
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(DEAD, glm::vec2(0.3750f, 0.125f * i));

	// Flag
	starSprite->setAnimationSpeed(FLAG_RIGHT, 8);
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(FLAG_RIGHT, glm::vec2(0.4375f, 0.125f * i));
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(FLAG_RIGHT, glm::vec2(0.5f, 0.125f * i));
	starSprite->setAnimationSpeed(FLAG_LEFT, 8);
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(FLAG_LEFT, glm::vec2(0.4375f, 0.5f + 0.125f * i));
	for (int i = 0; i < 4; ++i) starSprite->addKeyframe(FLAG_LEFT, glm::vec2(0.5f, 0.5f + 0.125f * i));

	starSprite->changeAnimation(0);
	starSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	/* ---------------------------- */
	/* ----- SUPER STAR MARIO ----- */
	/* ---------------------------- */

	superStarSpritesheet.loadFromFile("images/spriteSuperStarMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	superStarSprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.0625f, 0.125f), &superStarSpritesheet, &shaderProgram);
	superStarSprite->setNumberAnimations(13);

	/* Idle */
	superStarSprite->setAnimationSpeed(STAND_RIGHT, 4);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.125f*i));

	superStarSprite->setAnimationSpeed(STAND_LEFT, 4);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.5f + 0.125f*i));

	/* Moving */
	superStarSprite->setAnimationSpeed(MOVE_RIGHT, 6);
	for (int i = 0; i < 12; ++i) {
		int i_x = i % 3;
		int i_y = (i % 4);
		superStarSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f + 0.0625f * i_x, 0.125f * i_y));
	}
	superStarSprite->setAnimationSpeed(MOVE_LEFT, 6);
	for (int i = 0; i < 12; ++i) {
		int i_x = i % 3;
		int i_y = (i % 4);
		superStarSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f + 0.0625f * i_x, 0.5f + 0.125f * i_y));
	}

	/* Jumping */
	superStarSprite->setAnimationSpeed(JUMP_RIGHT, 4);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.125f*i));
	superStarSprite->setAnimationSpeed(JUMP_LEFT, 4);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(JUMP_LEFT, glm::vec2(0.3125f, 0.5f + 0.125f*i));

	/* Change Direction */
	superStarSprite->setAnimationSpeed(CHANGE_RIGHT, 4);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(CHANGE_RIGHT, glm::vec2(0.25f, 0.125f*i));
	superStarSprite->setAnimationSpeed(CHANGE_LEFT, 4);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(CHANGE_LEFT, glm::vec2(0.25f, 0.5f + 0.125f*i));

	/* DEAD (No existe) */
	superStarSprite->setAnimationSpeed(DEAD, 4);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(DEAD, glm::vec2(0.3750f, 0.125f*i));

	/* Crouched */
	superStarSprite->setAnimationSpeed(CROUCHED_RIGHT, 4);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(CROUCHED_RIGHT, glm::vec2(0.375f, 0.125f*i));
	superStarSprite->setAnimationSpeed(CROUCHED_LEFT, 4);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(CROUCHED_LEFT, glm::vec2(0.375f, 0.5f + 0.125f*i));

	// Flag
	superStarSprite->setAnimationSpeed(FLAG_RIGHT, 8);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(FLAG_RIGHT, glm::vec2(0.4375f, 0.125f * i));
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(FLAG_RIGHT, glm::vec2(0.5f, 0.125f * i));
	superStarSprite->setAnimationSpeed(FLAG_LEFT, 8);
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(FLAG_LEFT, glm::vec2(0.4375f, 0.5f + 0.125f * i));
	for (int i = 0; i < 4; ++i) superStarSprite->addKeyframe(FLAG_LEFT, glm::vec2(0.5f, 0.5f + 0.125f * i));

	superStarSprite->changeAnimation(0);
	superStarSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
}

void Player::update(int deltaTime, float xmin, float& xmax) {
	/* --- Mario Sprites --- */
	Sprite* activeSprite = sprite;
	if (superMario && !starMario)      activeSprite = superSprite;
	else if (starMario && !superMario) activeSprite = starSprite;
	else if (superMario && starMario)  activeSprite = superStarSprite;
	activeSprite->update(deltaTime);

	/* ----- Movement Keys ----- */
	bool keyLeft =  Game::instance().getSpecialKey(GLUT_KEY_LEFT)  || Game::instance().getKey('a') || Game::instance().getKey('A');
	bool keyRight = Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || Game::instance().getKey('d') || Game::instance().getKey('D');
	bool keyUp =	Game::instance().getSpecialKey(GLUT_KEY_UP)    || Game::instance().getKey(32)  ||
					Game::instance().getKey('w') || Game::instance().getKey('W');
	bool keyDown =	Game::instance().getSpecialKey(GLUT_KEY_DOWN)  || Game::instance().getKey('s') || Game::instance().getKey('S');
	bool keyRun =	Game::instance().getModifierKey(0);

	/* ---- Power-Up Keys ---- */
	bool keySuperMario = Game::instance().getKey('m') || Game::instance().getKey('M');
	bool keyStarMario = Game::instance().getKey('g') || Game::instance().getKey('G');

	/* Sprite Selection (Super Mario) */
	if (!superMario && !superMarioKey && keySuperMario && !bPowerUpAnimation && !bImmunity && state != DYING) {
		superMarioKey = true;
		if (starMario) superStarSprite->changeAnimation(activeSprite->animation());
		else superSprite->changeAnimation(activeSprite->animation());
		setSuperMario();
	}
	else if (superMario && !superMarioKey && keySuperMario && !bPowerUpAnimation && state != DYING) {
		superMarioKey = true;
		if (starMario) activeSprite->changeAnimation(starSprite->animation());
		else activeSprite->changeAnimation(sprite->animation());
		powerDown();
	}

	/* Sprite Selection (Star Mario) */
	if (starMario) starMarioTime += deltaTime;
	if (!starMario && !starMarioKey && keyStarMario && !bPowerUpAnimation && !bImmunity && state != DYING) {
		starMarioKey = true;
		if (superMario) superStarSprite->changeAnimation(activeSprite->animation());
		else starSprite->changeAnimation(activeSprite->animation());
		setStarMario();
	}
	else if (starMario && state != DYING && (starMarioTime > TIME_STAR_MARIO || (!starMarioKey && keyStarMario && !bPowerUpAnimation))) {
		starMario = false, starMarioKey = true;
		if (superMario) {
			activeSprite->changeAnimation(superSprite->animation());
			state = SUPER;
		}
		else {
			activeSprite->changeAnimation(sprite->animation());
			state = NORMAL;
		}
		sound->stopBGM();
		sound->playBGM("music/title.mp3", true);
	}

	/* PHYSICS */
	glm::ivec2 dimMario = glm::ivec2(32, 32);
	if (superMario) dimMario = glm::ivec2(32, 64);

	/* Check Fall Off Map */
	if (state != DYING && posPlayer.y > map->getMapSize().y * map->getBlockSize() + 64) {
		sprite->changeAnimation(DEAD);
		state = DYING;
		jumpAngle = 90;
		startY = posPlayer.y;
		sound->playSFX("sfx/mario_dies.wav");
	}

	/* ----- DYING ANIMATION ----- */
	if (state == DYING) {
		jumpAngle += JUMP_ANGLE_STEP;

		if (jumpAngle < 90) {
			posPlayer.y = int(startY - JUMP_HEIGHT_DYING * sin(3.14159f * jumpAngle / 180.f));
		}
		else {
			posPlayer.y += 4;
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		return;
	}

	/* --- Power-Up Animation (Power-Up to Super Mario) --- */
	if (bPowerUpAnimation) {
		timePowerUpAnimation += deltaTime;
		if (timePowerUpAnimation > TIME_POWER_UP_ANIMATION) {
			bPowerUpAnimation = false;
			superMario = true;
		}
		renderPowerUpAnimation = (renderPowerUpAnimation + 1) % 15;
		return;
	}

	/* ----- IMMUNITY (Power-Down from Super Mario) ----- */
	if (bImmunity) {
		timeImmunity += deltaTime;
		if (superMario && timeImmunity >= 0.2 * TIME_IMMUNE) superMario = false;
		if (timeImmunity > TIME_IMMUNE) bImmunity = false;
		renderImmunity = (renderImmunity + 1) % 15;
		if (timeImmunity < 0.2 * TIME_IMMUNE) return;
	}

	/* MOVEMENT */
	if (keyLeft && !(superMario && keyDown)) {
		if (!bJumping && !bFalling && activeSprite->animation() != MOVE_LEFT) {
			activeSprite->changeAnimation(MOVE_LEFT);
		}
		bool colision = false;
		if (!bJumping && !bFalling && Vx > 0) {
			activeSprite->changeAnimation(CHANGE_RIGHT);
			if (map->collisionMoveRight(posPlayer, dimMario, &posPlayer.x, superMario)) {
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
			Vx = 0;
			activeSprite->changeAnimation(STAND_LEFT);
		}
		else if (!colision) {
			posPlayer.x += Vx;
		}

		// Vx Correction
		if (Vx < -5) Vx = -5;
		else if (!keyRun && Vx < -3) Vx = -3;
	}

	else if (keyRight && !(superMario && keyDown)) {
		if (!bJumping && !bFalling && activeSprite->animation() != MOVE_RIGHT) {
			activeSprite->changeAnimation(MOVE_RIGHT);
		}
		bool colision = false;
		if (!bJumping && !bFalling && Vx < 0) {
			activeSprite->changeAnimation(CHANGE_LEFT);
			if (map->collisionMoveLeft(posPlayer, dimMario, &posPlayer.x, superMario)) {
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
			Vx = 0;
			activeSprite->changeAnimation(STAND_RIGHT);
		}
		else if (!colision) {
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
			if (activeSprite->animation() == CHANGE_LEFT || activeSprite->animation() == MOVE_LEFT ||
				activeSprite->animation() == CROUCHED_LEFT)
				activeSprite->changeAnimation(STAND_LEFT);
			else if (activeSprite->animation() == CHANGE_RIGHT || activeSprite->animation() == MOVE_RIGHT ||
				activeSprite->animation() == CROUCHED_RIGHT)
				activeSprite->changeAnimation(STAND_RIGHT);
		}

		if (map->collisionMoveRight(posPlayer, dimMario, &posPlayer.x, superMario)) Vx = 0;
		else if (map->collisionMoveLeft(posPlayer, dimMario, &posPlayer.x, superMario)) Vx = 0;
	}

	//al aplicar los respectivos velocidades hay que fijarse que no esta intersectado (hecho por jeremy)
	map->collisionMoveRight(posPlayer, dimMario, &posPlayer.x, superMario);
	map->collisionMoveLeft(posPlayer, dimMario, &posPlayer.x, superMario);

	if (bJumping) {
		if (jumpingEnemy) jumpAngle += JUMP_ANGLE_STEP_ENEMY;
		else {
			jumpAngle += JUMP_ANGLE_STEP;
		}

		if (jumpAngle >= 180) {
			bJumping = false; jumpingEnemy = false;
			posPlayer.y = startY;
			gravity = 0;
		}
		else {
			if (jumpingEnemy) posPlayer.y = int(startY - JUMP_HEIGHT_ENEMY * sin(3.14159f * jumpAngle / 180.f));
			else	          posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

			if (jumpAngle > 90) { // is falling down
				bFalling = true;
				bJumping = false;
				if (gravity < 4) gravity += 0.5f;
			}

			else bJumping = !map->collisionMoveUp(posPlayer, dimMario, &posPlayer.y, superMario);
			if (!bJumping) jumpingEnemy = false;

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
				if (superMario)	sound->playSFX("sfx/jump-super.wav");
				else sound->playSFX("sfx/jump-small.wav");
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

	if (keyDown) {
		if (superMario && !bJumping && !bFalling) {
			if (activeSprite->animation() == MOVE_LEFT || activeSprite->animation() == STAND_LEFT ||
				activeSprite->animation() == CHANGE_LEFT)
				activeSprite->changeAnimation(CROUCHED_LEFT);
			else if (activeSprite->animation() == MOVE_RIGHT || activeSprite->animation() == STAND_RIGHT ||
				activeSprite->animation() == CHANGE_RIGHT)
				activeSprite->changeAnimation(CROUCHED_RIGHT);
		}
	}
	else {
		if (activeSprite->animation() == CROUCHED_LEFT) activeSprite->changeAnimation(STAND_LEFT);
		else if (activeSprite->animation() == CROUCHED_RIGHT) activeSprite->changeAnimation(STAND_RIGHT);
	}

	/* Screen Scroll */
	if (posPlayer.x + 32 + tileMapDispl.x > xmax) {
		xmax = posPlayer.x + 32 + tileMapDispl.x;
	}
	else if (posPlayer.x < xmin && !keyRight) {
		posPlayer.x = xmin;
		Vx = 0;
	}

	/* --- Key State Updates --- */
	keyJumpPressed = keyUp;
	superMarioKey = keySuperMario;
	starMarioKey = keyStarMario;

	/* --- Sprite Update --- */
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	superSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
	starSprite-> setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	superStarSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));

	/* --- Prints --- */
	
	//if (keyLeft) std::cout << "LEFT" << endl;
	//if (keyRight) std::cout << "RIGHT" << endl;
	//if (keyUp) std::cout << "UP" << endl;
	//if (keyDown) std::cout << "DOWN" << endl;

	//std::cout << jumpAngle << endl;

	//if (bJumping) std::cout << "bJumping" << endl;
	//if (bFalling) std::cout << "bFalling" << endl;
	// 
	//if (bImmunity) std::cout << "bImmunity" << endl;
	//if (starMario) std::cout << "starMario" << endl;
	//std::cout << endl;
	//cout << posPlayer.x << " , " << posPlayer.y << " , " << endl;
}

void Player::render() {
	if (bImmunity && renderImmunity <= 6) return;

	if (bPowerUpAnimation) {
		superMario = renderPowerUpAnimation <= 6;
	}

	if (superMario && !starMario) superSprite->render();
	else if (!superMario && starMario) starSprite->render();
	else if (superMario && starMario) superStarSprite->render();
	else sprite->render();
}

void Player::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


void Player::setSuperMario() {
	superMario = true, state = SUPER;
	bPowerUpAnimation = true;
	timePowerUpAnimation = 0;
	renderPowerUpAnimation = 0;
	sound->playSFX("sfx/powerup-eats.wav");
}

void Player::setStarMario() {
	sound->stopBGM();
	sound->playSFX("sfx/powerup-eats.wav");
	sound->playBGM("music/star_mario.mp3", true);
	starMario = true, state = STAR;
	starMarioTime = 0;
}

void Player::powerDown() {
	state = NORMAL;
	sound->playSFX("sfx/powerdown-pipe.wav");
	timeImmunity = 0;
	bImmunity = true;
	renderImmunity = 0;
}

/* ENEMY COLLISIONS */
void Player::jumpEnemy() {
	bJumping = true;
	jumpingEnemy = true;
	jumpAngle = 0;
	startY = posPlayer.y;
}

void Player::setDying() {
	if (state == DYING) return;
	sprite->changeAnimation(DEAD);
	state = DYING;
	jumpAngle = 0;
	startY = posPlayer.y;
	sound->playSFX("sfx/mario_dies.wav");
}

void Player::collisionEnemy() {
	if (state == NORMAL) {
		sprite->changeAnimation(DEAD);
		state = DYING;
		jumpAngle = 0;
		startY = posPlayer.y;
		sound->playSFX("sfx/mario_dies.wav");
	}
	else if (state == SUPER) {
		powerDown();
	}
}

bool Player::isDead() {
	return state == DYING;
}

bool Player::isImmune() {
	return bImmunity;
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

bool Player::isStarMario() const {
	return starMario;
}
bool Player::isSuperMario() const {
	return superMario;
}
void Player::animationOfReachingFinal(int deltaTime) {
	bJumping = false;
	posPlayer.y += FALL_STEP;

	/* Sprite Updates */
	sprite->update(deltaTime); superSprite->update(deltaTime);
	starSprite->update(deltaTime);superStarSprite->update(deltaTime);

	/* Sprite Animation Selection */
	if (sprite->animation() != FLAG_RIGHT) sprite->changeAnimation(FLAG_RIGHT);
	if (superSprite->animation() != FLAG_RIGHT) superSprite->changeAnimation(FLAG_RIGHT);
	if (starSprite->animation() != FLAG_RIGHT) starSprite->changeAnimation(FLAG_RIGHT);
	if (superStarSprite->animation() != FLAG_RIGHT) superStarSprite->changeAnimation(FLAG_RIGHT);

	map->collisionMoveDown(posPlayer, glm::ivec2(32,32), &posPlayer.y);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	superSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
	starSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	superStarSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
}

void Player::setInitialStateSuperMario(bool superMario) {
	this->superMario = superMario;
	if (superMario) state = SUPER;
}

void Player::reachCastleAnimation(float dt) {
	if (auxFirstTime) {
		auxFirstTime = false;
		sound->playSFX("sfx/stage_clear.wav");
		sprite->changeAnimation(MOVE_RIGHT);
		superSprite->changeAnimation(MOVE_RIGHT);
		starSprite->changeAnimation(MOVE_RIGHT);
		superStarSprite->changeAnimation(MOVE_RIGHT);
	}

	sprite->update(dt);
	superSprite->update(dt);
	starSprite->update(dt);
	superStarSprite->update(dt);
	posPlayer.y += FALL_STEP;
	map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
	posPlayer.x += 2.;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	superSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
	starSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	superStarSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
};