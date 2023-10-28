#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Goomba.h"


#define SCREEN_X 0
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 11


Scene::Scene() {
	map = NULL;
	player = NULL;
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f));
	//score = NULL;
	enemies = vector<Enemy*>();
}

Scene::~Scene() {
	if (map != NULL) delete map;
	if (player != NULL) delete player;
	//if (score != NULL) delete score;

	for (auto enemy : enemies) {
		delete enemy;
	}
}

void Scene::init() {
	initShaders();
	map = TileMap::createTileMap("levels/prueba.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));
	currentTime = 0.0f;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	//score = new Score();
	Score::instance().init();//score->init();

	vector<glm::vec2> posEnemies;
	posEnemies.push_back(glm::vec2(22, 11)); posEnemies.push_back(glm::vec2(40, 11));
	posEnemies.push_back(glm::vec2(51, 11)); posEnemies.push_back(glm::vec2(52, 11));
	posEnemies.push_back(glm::vec2(80,  3)); posEnemies.push_back(glm::vec2(82,  3));
	posEnemies.push_back(glm::vec2(97, 11)); posEnemies.push_back(glm::vec2(99, 11));
	// posEnemies.push_back(glm::vec2(107, 11)); // TORTUGA
	posEnemies.push_back(glm::vec2(114, 11)); posEnemies.push_back(glm::vec2(116, 11));
	posEnemies.push_back(glm::vec2(124, 11)); posEnemies.push_back(glm::vec2(126, 11));
	posEnemies.push_back(glm::vec2(129, 11)); posEnemies.push_back(glm::vec2(131, 11));
	posEnemies.push_back(glm::vec2(174, 11)); posEnemies.push_back(glm::vec2(176, 11));

	/* Enemies */
	for (int i = 0; i < 6; ++i) {
		Goomba* e = new Goomba;
		e->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		e->setPosition(glm::vec2(posEnemies[i].x * map->getTileSize(), posEnemies[i].y * map->getTileSize()));
		e->setTileMap(map);
		enemies.push_back(e);
	}

	pause = true, keyPausePressed = false;
	playingMusic = false;
}

void Scene::restart() {
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	currentTime = 0.0f;
}

void Scene::change() {
	map = TileMap::createTileMap("levels/prueba.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));
	currentTime = 0.0f;

}

void Scene::update(int deltaTime, bool inMenu) {
	if (inMenu) return;
	if (!playingMusic) {
		sound.playBGM("music/title.mp3", true);
		playingMusic = true;
	}

	/* Pause Game */
	bool keyPause = Game::instance().getKey(13);
	if (keyPause && !keyPausePressed) {
		keyPausePressed = true;
		sound.playSFX("sfx/pause.wav");
		if (pause) pause = false;
		else {
			pause = true;
			return;
		}
	}
	keyPausePressed = keyPause;
	if (pause) return;


	currentTime += deltaTime;
	map->update(deltaTime);

	/* CAMERA */
	float actualMid = camera.getXmid();

	/* MARIO */
	player->update(deltaTime, camera.getXmin(), actualMid);

	/* ENEMIES */
	for (auto *enemy : enemies) {
		enemy->update(deltaTime, camera.getXmin());
		enemy->collision(player->getPos(), player->getSize());
	}

	/* SCORE */
	Score::instance().update(deltaTime);

	/* CAMERA POSITION UPDATE */
	if (actualMid != camera.getXmid()) {
		camera.setMidXPosition(actualMid);
	}
}

void Scene::render() {
	glm::mat4 modelview;

	texProgram.use();
	camera.bindProjection(texProgram);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render(camera.getPosition(), camera.getSize());

	/* MARIO */
	player->render();

	/* ENEMIES */
	for (auto* enemy : enemies) {
		enemy->render();
	}
	Score::instance().render();//score->render();
}

void Scene::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled()) {
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled()) {
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



