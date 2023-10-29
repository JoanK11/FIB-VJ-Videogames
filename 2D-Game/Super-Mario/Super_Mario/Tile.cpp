#include "Tile.h"
Tile::Tile(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& program, Texture* text) {

	spr = Sprite::createSprite(tileSize, textureSize, text, &program);
	spr->setNumberAnimations(1);
	spr->setAnimationSpeed(0, 1);
	spr->addKeyframe(0, texturePosition);
	spr->changeAnimation(0);
	spr->setPosition(tilePosition);
	pos = tilePosition;
}
Tile::Tile(){}

Tile::~Tile() {}

void Tile::render() {
	spr->render();

}
bool Tile::isTouchable() {
	return false;
}
void Tile::update(float dt) {}

Object* Tile::actionToTouch(bool isSuperMario) {
	return nullptr;
}