#pragma once

#include "Sprite.h"
#include "Score.h"
class Flag
{
private:
	Sprite* spr;
	int ymin, ymax, xlim;
	int yminOriginal, ymaxOriginal, xlimOriginal;
	glm::vec2 tileMapDisplay;
	enum FlagState {
		IDLE,
		ANIMATION,
		END
	};
	FlagState currentState;
public:
	Flag(int ymin, int ymax, int xlim, const glm::vec2& tileMapDisplay, ShaderProgram *p);
	bool touchTheFlag(const glm::ivec2& pos, const glm::ivec2& size, bool superMario);
	bool update(float dt);
	void render();
	void restart();
};

