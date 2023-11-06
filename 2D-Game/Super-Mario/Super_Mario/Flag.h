#pragma once

#include "Sprite.h"
#include "Score.h"
#include "Text.h"
#include "Texture.h"

#define TIME_TEXT 1500

class Flag {
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

	Text text;
	glm::ivec2 textPos;
	int score;
	float timeText;
	bool showingText;
public:
	Flag(int ymin, int ymax, int xlim, const glm::vec2& tileMapDisplay, ShaderProgram *p);
	bool touchTheFlag(const glm::ivec2& pos, const glm::ivec2& size, bool superMario);
	bool update(float dt);
	void render(glm::vec2& cameraPos);
	void restart();
	void showText(glm::vec2& posPlayer);
};

