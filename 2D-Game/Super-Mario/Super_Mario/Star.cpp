#include "Star.h"
#include "Player.h"
#include "TileMap.h"
#define TIME_ANIMATION 1500
#define FALL_STEP 4
#define JUMP_ANGLE_STEP 4
#define WAITING_TIME 250
Star::Star(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& tileMapDisplay, TileMap* m, ShaderProgram* p, float Vx) : Object(pos, size, tileMapDisplay, m) {

	this->y_size = size.y;
	this->Vx = Vx;
	this->originalVx = Vx;
	Texture* text = new Texture();
	text->loadFromFile("images/stars.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spr = Sprite::createSprite(size, glm::vec2(0.25, 1.0), text, p);
	program = p;
	spr->setNumberAnimations(1);
	spr->setAnimationSpeed(0, 8);
	spr->addKeyframe(0, glm::vec2(0., 0.));
	spr->addKeyframe(0, glm::vec2(0.25, 0.));
	spr->addKeyframe(0, glm::vec2(0.50, 0.));
	spr->addKeyframe(0, glm::vec2(0.75, 0.));
	spr->changeAnimation(0);
	currentTime = 0;
	currentState = WAITING;
	bJumping = true;
	jumpAngle = 0;
	startY = pos.y;

}
void Star::restart() {
	Object::restart();
	Vx = originalVx;
	currentTime = 0;
	currentState = WAITING;
	bJumping = true;
	jumpAngle = 0;
	startY = pos.y;
}

void Star::actionOfObject(Player* ply) {
	if (currentState == ANIMATION) return;
	else if (currentState == VALID) {
		ply->setStarMario();
		currentState = USED;
	}
	else return;


}
bool Star::collide(const glm::vec2& plyPos, const glm::vec2& plySize) {
	return currentState == VALID && Object::collide(plyPos, plySize);
}

void Star::update(float dt) {
	if (currentState == WAITING) {
		currentTime += dt;
		if (currentTime >= WAITING_TIME) {
			currentState = ANIMATION;
			currentTime = 0;
			sound.playSFX("sfx/powerup-appears.wav");
		}
	}
	else if (currentState == ANIMATION) {
		currentTime += dt;
		float y = -y_size * sin(3.141592 / 180. * (90. / TIME_ANIMATION) * currentTime);
		glm::vec2 p = pos + tileMapDisplay;
		p.y += y;
		spr->setPosition(p);
		if (currentTime >= TIME_ANIMATION) {
			pos.y += y;
			currentState = VALID;
			startY = pos.y;
		}

	}
	else if (currentState == VALID) {
		spr->update(dt);
		if (!map->isInside(pos, size)) {
			currentState = USED;
			return;
		}
		
		if (bJumping) {
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180) {
				bJumping = false;
				pos.y = startY;
			}
			else {
				pos.y = int(startY - 64 * sin(3.14159f * jumpAngle / 180.f));

				if (jumpAngle > 90) { // is falling down
					bJumping = !map->collisionMoveDown(pos, size, &pos.y);

					//if (!bJumping) pos.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				}
				else bJumping = !map->collisionMoveUp(pos, size, &pos.y);

			}
		}
		else {
			pos.y += FALL_STEP;

			if (map->collisionMoveDown(pos, size, &pos.y)) {
				bJumping = true;
				jumpAngle = 0;
				startY = pos.y;
			}
		}

		pos.x += Vx;
		if (Vx > 0 && map->collisionMoveRight(pos, size, &pos.x)) Vx = -Vx;
		else if (Vx < 0 && map->collisionMoveLeft(pos, size, &pos.x)) Vx = -Vx;
		spr->setPosition(pos + tileMapDisplay);
	}
}
void Star::render(glm::vec2& cameraPos) {
	if (currentState == ANIMATION) {	
		spr->render();
	}
	else if (currentState == VALID) spr->render();

}