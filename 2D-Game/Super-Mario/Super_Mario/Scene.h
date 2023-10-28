#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Projection.h"
#include "Score.h"
#include "SoundManager.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene {

public:
	Scene();
	~Scene();

	void init();
	void restart();
	void change();
	void update(int deltaTime, bool inMenu);
	void render();

private:
	void initShaders();

private:
	SoundManager sound;


	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	bool pause, keyPausePressed;

	Projection camera;
	glm::mat4 projection;
	Score *score;
	
};


#endif // _SCENE_INCLUDE

