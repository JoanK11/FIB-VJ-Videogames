#pragma once

#include<glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Texture.h"
#include "ShaderProgram.h"

class Tile
{
	protected:
		

		bool isWall;

		Texture* text;

		GLuint vao;
		GLuint vbo;

		GLuint posLocation;
		GLuint texCoordLocation;

	
public:
	Tile(glm::vec2 tilePosition, glm::vec2 tileSize, glm::vec2 texturePosition, glm::vec2 textureSize, ShaderProgram& shader,  Texture* text);
	
	virtual ~Tile();

	virtual void render();
	
	virtual bool isTouchable();
	


};

