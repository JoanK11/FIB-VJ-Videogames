#include "Projection.h"
Projection::Projection(){}

Projection::Projection(const glm::vec2& position, const glm::vec2& size) {
	this->pos = position;
	this->size = size;
	this->displacement = glm::vec2(0., 0.);
	projectionMatrix = glm::translate(glm::mat4(1.0),glm::vec3(0., 0., 0.));
}
bool Projection::surpassLeft(const glm::vec2& playerPos, const glm::vec2& PlayerSize) const {
	return playerPos.x < pos.x;
}
void Projection::setPosition( glm::vec2& newPosition) {
	this->pos = newPosition;
	projectionMatrix = glm::ortho(pos.x, pos.x + size.x, pos.y + size.y, pos.y);
}
void Projection::setMidPosition(const glm::vec2& playerPos, const glm::vec2& PlayerSize) {
	float xPlayer = playerPos.x + PlayerSize.x;
	float midSize = size.x / 2.;
	float newX = xPlayer - midSize;
	setPosition(glm::vec2(newX, pos.y));
}
bool Projection::surpassMiddleLeft(const glm::vec2& playerPos, const glm::vec2& PlayerSize) const {
	float xPlayer = playerPos.x + PlayerSize.x;
	float mid = pos.x + size.x / 2.;
	return xPlayer > mid;

}
void Projection::bindProjection(ShaderProgram& program) {
	program.setUniformMatrix4f("viewMatrix", projectionMatrix);
}
float Projection::getXmin() {
	return pos.x;
}
float Projection::getXmax() {
	return pos.x + size.x;
}
float Projection::getXmid() {
	return pos.x + 0.5f * size.x;
}

void Projection::setMidXPosition(float newXMidPos) {
	float newpos = newXMidPos - 0.5f * size.x;
	displacement.x += (newpos - pos.x);
	pos.x = newpos;
	projectionMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-displacement.x, -displacement.y, 0));
}
glm::vec2 Projection::getPosition() {
	return pos;
}
glm::vec2 Projection::getSize() {
	return size;
}