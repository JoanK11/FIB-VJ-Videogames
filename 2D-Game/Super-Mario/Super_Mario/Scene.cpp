#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 11


Scene::Scene() {
	map = NULL;
	player = NULL;
	projection = Projection(glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f));
	score = NULL;
}

Scene::~Scene() {
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}


void Scene::init() {
	initShaders();
	map = TileMap::createTileMap("levels/prueba.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));
	currentTime = 0.0f;
	score = new Score();
	score->init();
}

void Scene::change() {
	map = TileMap::createTileMap("levels/prueba.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));
	currentTime = 0.0f;
}

void Scene::update(int deltaTime) {
	currentTime += deltaTime;
	float actualMid = projection.getXmid();
	player->update(deltaTime, projection.getXmin(), actualMid);
	if (actualMid != projection.getXmid()) {
		projection.setMidXPosition(actualMid);
	}
}

void Scene::render() {
	glm::mat4 modelview;

	texProgram.use();
	projection.bindProjection(texProgram);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render(projection.getPosition(), projection.getSize());
	player->render();
	score->render();
}

void Scene::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
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



