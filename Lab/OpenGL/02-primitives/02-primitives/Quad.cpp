#include <GL/glew.h>
#include <GL/gl.h>
#include "Quad.h"


Quad *Quad::createQuad(float x, float y, float width, float height, ShaderProgram &program)
{
	Quad *quad = new Quad(x, y, width, height, program);

	return quad;
}


Quad::Quad(float x, float y, float width, float height, ShaderProgram &program)
{
	float vertices[18] = {x, y, x + width, y, x + width, y + height, x, y, x + width, y + height, x, y + height, x, y + height, x + width, y + height, x + width*0.5, y + height*1.5};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2);
}

void Quad::render() const
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glDrawArrays(GL_TRIANGLES, 0, 9);
}

void Quad::free()
{
	glDeleteBuffers(1, &vbo);
}

