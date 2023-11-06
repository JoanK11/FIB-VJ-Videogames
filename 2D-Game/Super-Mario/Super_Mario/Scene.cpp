#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 192
#define INIT_PLAYER_Y_TILES 11

#define TIME_GAME_OVER 7000


Scene::Scene() {
	map = NULL;
	player = NULL;
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f));
}

Scene::~Scene() {
	if (map != NULL) delete map;
	if (player != NULL) delete player;


}

void Scene::init() {
	initShaders();
	maps = vector<TileMap*>(2, nullptr);
	maps[0] = TileMap::createTileMap("levels/world1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	maps[1] = TileMap::createTileMap("levels/world2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = maps[0];
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getBlockSize(), INIT_PLAYER_Y_TILES * map->getBlockSize()));
	player->setTileMap(map);
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));
	currentTime = 0.0f;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	
	/* Score */
	Score::instance().init();

	/* Start Menu */
	startMenu.init();


	pause = false, keyPausePressed = false;
	playingMusic = false; gameOver = false;

	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font!!!" << endl;
	}
}

void Scene::restart() {
	currentTime = 0.0f;

	/* --- Player --- */
	delete player;
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getBlockSize(), INIT_PLAYER_Y_TILES * map->getBlockSize()));
	player->setTileMap(map);

	/* --- Camera --- */
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));

	/* --- Map --- */
	map->restart();

	/* --- Score --- */
	Score::instance().decreaseLive();
	Score::instance().restart();

	

	/* --- Sound --- */
	sound.stopBGM();
	if (Score::instance().gameOver()) {
		sound.playSFX("sfx/game_over.wav");
		gameOver = true;
		timeGameOver = 0;
		Game::instance().clearInput();
	}
	else sound.playBGM("music/title.mp3", true);
}

void Scene::change() {
	currentTime = 0.0f;

	/* --- Player --- */
	bool isSuperMario = player->isSuperMario();
	delete player;
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getBlockSize(), INIT_PLAYER_Y_TILES * map->getBlockSize()));
	player->setTileMap(map);
	player->setInitialStateSuperMario(isSuperMario);
	/* --- Camera --- */
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));

	/* --- Map --- */
	map->restart();

	/* --- Score --- */
	
	Score::instance().restart();



	/* --- Sound --- */
	sound.stopBGM();
	if (Score::instance().gameOver()) {
		sound.playSFX("sfx/game_over.wav");
		gameOver = true;
		timeGameOver = 0;
		Game::instance().clearInput();
	}
	else sound.playBGM("music/title.mp3", true);
}

void Scene::update(int deltaTime) {
	/* --- Menu --- */
	startMenu.update(deltaTime);
	bool inMenu = startMenu.showingMenu();
	if (inMenu) return;

	/* --- Game Over --- */
	if (gameOver) {
		timeGameOver += deltaTime;
		if (timeGameOver > TIME_GAME_OVER) {
			gameOver = false, pause = false;
			startMenu.openMenu();
			playingMusic = false;
			glClearColor(0.3607843137f, 0.5803921569f, 0.9882352941f, 1.0f);
			Score::instance().restartLives();
		}
		return;
	}

	/* --- Pause --- */
	bool keyPause = Game::instance().getKey(13);
	if (keyPause && !keyPausePressed && playingMusic) {
		keyPausePressed = true;
		sound.playSFX("sfx/pause.wav");
		if (pause) {
			pause = false;
			sound.resumeBGM();
		}
		else {
			pause = true;
			sound.pauseBGM();
			return;
		}
	}
	keyPausePressed = keyPause;
	if (pause) return;

	

	/* --- Music --- */
	if (!playingMusic) {
		sound.playBGM("music/title.mp3", true);
		playingMusic = true;
	}

	/*CHECKING IF REACHED THE FINISH LINE*/
	if (map->reachFinishLine(player->getPos(), player->getSize(), player->isSuperMario())) {
		map->animationOfFlag(deltaTime);
		player->animationOfReachingFinal();
		return;
	}

	checkWorldKeys();


	/* --- Time --- */
	currentTime += deltaTime;

	/* --- Map --- */
	map->update(deltaTime);
	if (!player->isDead()) map->collisionWithItems(player);

	/* --- Camera --- */
	float actualMid = camera.getXmid();

	/* --- Player --- */
	player->update(deltaTime, camera.getXmin(), actualMid, sound);

	/* --- Enemies --- */
	map->updateEnemies(deltaTime, player, camera.getXmin(), actualMid);

	/* --- Music --- */
	if (player->isDead()) sound.stopBGM();

	/* --- Score --- */
	if (!player->isDead()) Score::instance().update(deltaTime);

	/* --- Camera Position Update --- */
	if (actualMid != camera.getXmid()) {
		camera.setMidXPosition(actualMid);
	}

	/* --- Restart --- */
	glm::vec2 pos = player->getPos();
	if (pos.y > 1200) { //if (pos.y + size.y >= map->getBlockSize() * map->getMapSize().y) {
		restart();
	}
}

void Scene::render() {
	if (gameOver && !startMenu.showingMenu()) {
		glClearColor(0, 0, 0, 1.0f);
		text.render("GAME OVER", glm::vec2(SCREEN_WIDTH/2-90, SCREEN_HEIGHT/2), 20, glm::vec4(1, 1, 1, 1));
		Score::instance().render();
		return;
	}

	glm::mat4 modelview;
	texProgram.use();
	camera.bindProjection(texProgram);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	/* Map */
	map->render(camera.getPosition(), camera.getSize(), texProgram);

	/* Player */
	texProgram.use();
	player->render();

	/* Score */
	Score::instance().render();

	/* Start Menu */
	startMenu.render();
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
void Scene::checkWorldKeys() {
	bool world1 = Game::instance().getKey('1');
	bool world2 = Game::instance().getKey('2');
	if (world1 && map != maps[0]) {
		map = maps[0];
		Score::instance().updateWorld(1, 1);
		change();
	}
	else if (world2 && map != maps[1]) {
		map = maps[1];
		Score::instance().updateWorld(2, 1);
		change();
	}
}