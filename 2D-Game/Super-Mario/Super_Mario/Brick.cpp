#include "Brick.h"
Brick::Brick(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& shader, Texture* text) :
	Tile(tilePosition, tileSize, texturePosition, textureSize, shader, text) {
	isDestroyed = false;
	isWall = true;
}
Brick::~Brick() {}

void Brick::render(){
	if (!isDestroyed) {
		Tile::render();
	}
}
bool Brick::isTouchable() {
	return  !isDestroyed && isWall;
}