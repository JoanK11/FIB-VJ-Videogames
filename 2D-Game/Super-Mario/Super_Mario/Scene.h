#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Projection.h"
#include "Score.h"
#include "SoundManager.h"
#include "Enemy.h"
#include <vector>
#include "Text.h"
#include "StartMenu.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene {

public:
	Scene();
	~Scene();

	void init();
	void restart();
	void change();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	SoundManager sound;
	vector<Enemy*> enemies;
	StartMenu startMenu;


	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	Text text;
	float currentTime;
	bool pause, keyPausePressed;
	bool playingMusic;

	/* Game Over */
	bool gameOver;
	float timeGameOver;

	Projection camera;
	glm::mat4 projection;
	//Score *score;

	
	
};


#endif // _SCENE_INCLUDE

