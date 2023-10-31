#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Score.h"

void Score::init() {
	score = 0, lastScore = 0, coins = 0;
	world = make_pair(1, 1);
	time = 400; lives = 10;

	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font!!!" << endl;
	}
}

void Score::restart() {
	score = lastScore;
	time = 400;
}

void Score::update(int deltaTime) {
	time -= (deltaTime/800.f);
}

void Score::render() {
	/* SCORE */
	text.render("SCORE", glm::vec2(32, 32), 20, glm::vec4(1, 1, 1, 1));
	text.render(to_string(score), glm::vec2(78, 56), 20, glm::vec4(1, 1, 1, 1));

	/* COINS */
	text.render("COINS", glm::vec2(208, 32), 20, glm::vec4(1, 1, 1, 1));
	text.render(to_string(coins), glm::vec2(256, 56), 22, glm::vec4(1, 1, 1, 1));

	/* WORLD */
	text.render("WORLD", glm::vec2(384, 32), 20, glm::vec4(1, 1, 1, 1));
	text.render(to_string(world.first) + "-" + to_string(world.second), glm::vec2(408, 56), 20, glm::vec4(1, 1, 1, 1));

	/* TIME */
	text.render("TIME", glm::vec2(560, 32), 20, glm::vec4(1, 1, 1, 1));
	text.render(to_string(int(time)), glm::vec2(572, 56), 20, glm::vec4(1, 1, 1, 1));

	/* LIVES */
	text.render("LIVES", glm::vec2(704, 32), 20, glm::vec4(1, 1, 1, 1));
	text.render(to_string(lives), glm::vec2(736, 56), 20, glm::vec4(1, 1, 1, 1));
}


void Score::increaseScore(int x) {
	score += x;
}

void Score::increaseCoins() {
	++coins;
}

void Score::updateWorld(int x, int y) {
	world = make_pair(x, y);
}

void Score::decreaseLive() {
	--lives;
}

