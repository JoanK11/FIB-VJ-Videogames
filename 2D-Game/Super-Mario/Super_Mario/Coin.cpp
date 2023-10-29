#include "Coin.h"
#include "Player.h"
#define ANIMATION_TIME 3000.f

Coin::Coin(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& tileMapDisplay, TileMap* m, ShaderProgram* p):Object(pos,size,tileMapDisplay,m) {


	this->pos.y -= this->size.y;
	Texture* text = new Texture();
	text->loadFromFile("images/coins.png", TEXTURE_PIXEL_FORMAT_RGBA);

	spr = Sprite::createSprite(size, glm::vec2(0.25, 1.), text, p);
	spr->setNumberAnimations(1);
	spr->setAnimationSpeed(0, 4);
	spr->addKeyframe(0, glm::vec2(0., 0.));
	spr->addKeyframe(0, glm::vec2(0.25, 0.));
	spr->addKeyframe(0, glm::vec2(0.50, 0.));
	spr->addKeyframe(0, glm::vec2(0.75, 0.));
	spr->changeAnimation(0);
	currentTime = 0;
	currentState = NOT_ACHIEVED;
}

void Coin::actionOfObject(Player* ply) {
	//we add xx quantity to the score

	//we change the currentState for making
	currentState = ANIMATION;
}
bool Coin::collide(const glm::vec2& plyPos, const glm::vec2& plySize) {
	if (currentState == NOT_ACHIEVED) return true;
	return false;
}
void Coin::update(float dt) {
	if (currentState == ANIMATION) {

		currentTime += dt;
		spr->update(dt);
		float ys = -96 * sin(3.1415 / ANIMATION_TIME * currentTime);
		glm::vec2 p = pos + tileMapDisplay;
		p.y = p.y + ys;
		spr->setPosition(p);
		if (currentTime > ANIMATION_TIME){
			currentState = DISAPPEARD;
		}
	}
}
void Coin::render() {
	if (currentState == ANIMATION) {
		spr->render();

	}
}

