#include "Object.h"
Object::Object(glm::vec2 pos, glm::vec2 size, Texture* t, ShaderProgram& p) {
	float vertices[24] = { pos.x, pos.y, 0., 0.,
						   pos.x + size.x, pos.y,1., 0.,
						   pos.x + size.x, pos.y + size.y, 1., 1.,
						   pos.x, pos.y, 0., 0.,
						   pos.x + size.x, pos.y + size.y, 1., 1.0,
						   pos.x, pos.y + size.y, 0., 1. };
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = p.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = p.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	this->text = t;
	used = false;
}
void Object::render() {
	if (!used) {
		glEnable(GL_TEXTURE_2D);
		text->use();
		glBindVertexArray(vao);
		glEnableVertexAttribArray(posLocation);
		glEnableVertexAttribArray(texCoordLocation);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_TEXTURE_2D);
	}
}
bool Object::collide(glm::vec2 pos, glm::vec2 size) {
	return false;
}
