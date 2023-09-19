#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	scale = 1.0f;
	colStop = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render(scale, quadStart, colStop);
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		bPlay = false;
	keys[key] = true;

	if (key == 'q' && scale > 0.0f) {
		scale -= 0.05f;
	}

	else if (key == 'e' && scale < 5.0f) {
		scale += 0.05f;
	}

	else if (key == 'w') {
		quadStart = quadStart == 3 ? 0 : quadStart + 1;
	}

	else if (key == 's') {
		quadStart = quadStart == 0 ? 3 : quadStart - 1;
	}

	else if (key == 'z') {
		colStop = !colStop;
	}
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





