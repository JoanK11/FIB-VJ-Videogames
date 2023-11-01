#include "Mushroom.h"
#include "Player.h"
#include "TileMap.h"
#define TIME_ANIMATION 1250
#define FALL_STEP 4
#define WAITING_TIME 250

Mushroom::Mushroom(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& tileMapDisplay, TileMap* m, ShaderProgram* p, float Vx) : Object(pos,size,tileMapDisplay,m){
	
	this->y_size = size.y;
	this->Vx = Vx;
	Texture* text = new Texture();
	text->loadFromFile("images/mushroom.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spr = Sprite::createSprite(size, glm::vec2(1.0, 1.0), text, p);
	program = p;
	spr->setNumberAnimations(1);
	spr->setAnimationSpeed(0, 10);
	spr->addKeyframe(0, glm::vec2(0., 0.));
	spr->changeAnimation(0);
	currentTime = 0;
	currentState = WAITING;

}
void Mushroom::actionOfObject(Player* ply) {
	if (currentState == ANIMATION) return;
	else if (currentState == VALID) {
		ply->setSuperMario();
		currentState = USED;
	}
	else return;
	

}
void Mushroom::update(float dt) {
	if (currentState == WAITING) {
		currentTime += dt;
		if (currentTime >= WAITING_TIME) {
			currentState = ANIMATION;
			currentTime = 0;
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
		}

	}
	else if (currentState == VALID) {
		if (!map->isInside(pos, size)) {
			currentState = USED;
			return;
		}
		pos.y += FALL_STEP;
		map->collisionMoveDown(pos, size, &pos.y);

		pos.x += Vx;
		if (Vx > 0 && map->collisionMoveRight(pos, size, &pos.x)) Vx = -Vx;
		else if (Vx < 0 && map->collisionMoveLeft(pos, size, &pos.x)) Vx = -Vx;
		spr->setPosition(pos + tileMapDisplay);
	}
}
void Mushroom::render() {
	if (currentState == ANIMATION) {
		spr->render();
	}
	else if (currentState == VALID) spr->render();
		
}

bool Mushroom::collide(const glm::vec2& plyPos, const glm::vec2& plySize) {
	return currentState == VALID && Object::collide(plyPos, plySize);
}

void Mushroom::restart() {
	Object::restart();
	currentState = WAITING;
	currentTime = 0;
	spr->setPosition(pos);
}