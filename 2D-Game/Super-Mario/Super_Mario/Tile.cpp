#include "Tile.h"
Tile::Tile(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& program, Texture* text) {

	isWall = false;

	float vertices[24] = { tilePosition.x, tilePosition.y, texturePosition.x, texturePosition.y,
						   tilePosition.x + tileSize.x, tilePosition.y, texturePosition.x + textureSize.x, texturePosition.y,
						   tilePosition.x + tileSize.x, tilePosition.y + tileSize.y, texturePosition.x + textureSize.x, texturePosition.y + textureSize.y,
						   tilePosition.x, tilePosition.y, texturePosition.x, texturePosition.y,
						   tilePosition.x + tileSize.x, tilePosition.y + tileSize.y, texturePosition.x + textureSize.x, texturePosition.y + textureSize.y,
						   tilePosition.x, tilePosition.y + tileSize.y, texturePosition.x, texturePosition.y + textureSize.y };
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	this->text = text;
}
Tile::Tile(){}

Tile::~Tile() {}

void Tile::render() {
	glEnable(GL_TEXTURE_2D);
	text->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);

}
bool Tile::isTouchable() {
	return isWall;
}
void Tile::update(float dt) {}