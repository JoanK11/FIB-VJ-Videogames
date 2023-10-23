#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
#include "Player.h"
#include "ShaderProgram.h"
class Object
{
protected:
	bool used;
	Texture* text;

	GLuint vao;
	GLuint vbo;

	GLuint posLocation;
	GLuint texCoordLocation;

	glm::vec2 position;
	glm::vec2 size;
public:
	Object(glm::vec2 pos, glm::vec2 size, Texture* t, ShaderProgram& p);
	//virtual void actionObject(Player* pl) = 0;
	bool collide(glm::vec2 pos, glm::vec2 size);
	void render();
};

