#include "Object.h"
#include "TileMap.h"
#include "Player.h"
#include <iostream>

Object::Object(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& tileMapDisplay, TileMap* m) {
	this->pos = pos;
	originalPos = pos;
	this->size = size;
	this->tileMapDisplay = tileMapDisplay;
	map = m;
	showingText = false;

	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font!!!" << endl;
	}

}

bool Object::collide(const glm::vec2& plyPos, const glm::vec2& plySize) {
	float minx1 = pos.x;
	float miny1 = pos.y;
	float maxx1 = pos.x + size.x;
	float maxy1 = pos.y + size.y;

	float minx2 = plyPos.x;
	float miny2 = plyPos.y;
	float maxx2 = plyPos.x + plySize.x;
	float maxy2 = plyPos.y + plySize.y;

	bool a = (minx1 < maxx2) && (minx2 < maxx1);
	bool b = (miny1 < maxy2) && (miny2 < maxy1);

	return a && b;
}

void Object::restart() {
	pos = originalPos;
}

void Object::showText() {
	showingText = true;
	timeText = 0;
	textPos = pos;
}