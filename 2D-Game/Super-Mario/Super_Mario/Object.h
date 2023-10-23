#pragma once
#include <glm/glm.hpp>
#include "Texture.h"


class Object
{
private:
	bool isWall;

	Texture* text;

	GLuint vao;
	GLuint vbo;

	GLuint posLocation;
	
	GLuint texCoordLocation;
};

