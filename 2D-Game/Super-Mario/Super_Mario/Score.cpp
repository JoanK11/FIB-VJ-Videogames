#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Score.h"

void Score::init() {
	score = 0; coins = 0;
	world = make_pair(1, 1);
	time = 400; lives = 10;

	if (!text.init("fonts/super-mario-bros-nes.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Score::update(int deltaTime) {

}

void Score::render() {
	/* SCORE */
	text.render("SCORE", glm::vec2(50, 50), 32, glm::vec4(1, 1, 1, 1));
	//text.render(to_string(score), glm::vec2(10, 32), 32, glm::vec4(1, 1, 1, 1));

	/* COINS */
	//text.render("COINS", glm::vec2(10, 32), 32, glm::vec4(1, 1, 1, 1));
	//text.render(to_string(coins), glm::vec2(10, 32), 32, glm::vec4(1, 1, 1, 1));

	/* WORLD */
	//text.render("WORLD", glm::vec2(10, 32), 32, glm::vec4(1, 1, 1, 1));
	//text.render(to_string(world.first) + "-" + to_string(world.second), glm::vec2(10, 32), 32, glm::vec4(1, 1, 1, 1));

	/* TIME */
	//text.render("TIME", glm::vec2(10, 32), 32, glm::vec4(1, 1, 1, 1));
	//text.render(to_string(time), glm::vec2(10, 32), 32, glm::vec4(1, 1, 1, 1));

	/* LIVES */
	//text.render("LIVES", glm::vec2(10, 32), 32, glm::vec4(1, 1, 1, 1));
	//text.render(to_string(lives), glm::vec2(10, 32), 32, glm::vec4(1, 1, 1, 1));
}