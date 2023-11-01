#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init() {
	bPlay = true;
	glClearColor(0.3607843137f, 0.5803921569f, 0.9882352941f, 1.0f);
	clearInput();
	scene.init();
}

bool Game::update(int deltaTime) {
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key) {
	//if (key == 27) // Escape code
	//	bPlay = false;
	keys[key] = true;

	int modifiers = glutGetModifiers();
	modifierKeys[0] = modifiers & GLUT_ACTIVE_SHIFT;
	modifierKeys[1] = modifiers & GLUT_ACTIVE_CTRL;
	modifierKeys[2] = modifiers & GLUT_ACTIVE_ALT;
}

void Game::keyReleased(int key) {
	keys[key] = false;

	int modifiers = glutGetModifiers();
	modifierKeys[0] = modifiers & GLUT_ACTIVE_SHIFT;
	modifierKeys[1] = modifiers & GLUT_ACTIVE_CTRL;
	modifierKeys[2] = modifiers & GLUT_ACTIVE_ALT;
}

void Game::specialKeyPressed(int key) {
	specialKeys[key] = true;

	int modifiers = glutGetModifiers();
	modifierKeys[0] = modifiers & GLUT_ACTIVE_SHIFT;
	modifierKeys[1] = modifiers & GLUT_ACTIVE_CTRL;
	modifierKeys[2] = modifiers & GLUT_ACTIVE_ALT;
}

void Game::specialKeyReleased(int key) {
	specialKeys[key] = false;

	int modifiers = glutGetModifiers();
	modifierKeys[0] = modifiers & GLUT_ACTIVE_SHIFT;
	modifierKeys[1] = modifiers & GLUT_ACTIVE_CTRL;
	modifierKeys[2] = modifiers & GLUT_ACTIVE_ALT;
}

void Game::mouseMove(int x, int y) {
}

void Game::mousePress(int button) {
}

void Game::mouseRelease(int button) {
}

bool Game::getKey(int key) const {
	return keys[key];
}

bool Game::getSpecialKey(int key) const {
	return specialKeys[key];
}

bool Game::getModifierKey(int key) const {
	return modifierKeys[key];
}

void Game::clearInput() {
	for (int i = 0; i < 256; ++i) {
		keys[i] = false;
		specialKeys[i] = false;
	}
	
	for (int i = 0; i < 3; ++i) {
		modifierKeys[i] = false;
	}
}


