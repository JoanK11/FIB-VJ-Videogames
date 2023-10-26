#pragma once
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
class Projection
{
private:
	glm::vec2 pos;
	glm::vec2 size;
	glm::mat4 projectionMatrix;
	glm::vec2 displacement;
public:
	Projection();
	Projection(const glm::vec2& position, const glm::vec2& size);
	bool surpassLeft(const glm::vec2& playerPos, const glm::vec2& PlayerSize) const;
	void setPosition(glm::vec2& newPosition);
	void setMidPosition(const glm::vec2& playerPos, const glm::vec2& PlayerSize);
	void setMidXPosition(float newXMidPos);
	bool surpassMiddleLeft(const glm::vec2& playerPos, const glm::vec2& PlayerSize) const;
	void bindProjection(ShaderProgram& program);
	float getXmin();
	float getXmax();
	float getXmid();
	glm::vec2 getPosition();
	glm::vec2 getSize();
};

