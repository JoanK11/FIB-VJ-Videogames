#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Goomba.h"
#include "Koopa.h"

#define SCREEN_X 0
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 11

#define TIME_GAME_OVER 7000


Scene::Scene() {
	map = NULL;
	player = NULL;
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f));
	enemies = vector<Enemy*>();
}

Scene::~Scene() {
	if (map != NULL) delete map;
	if (player != NULL) delete player;

	for (auto enemy : enemies) {
		delete enemy;
	}
}

void Scene::init() {
	initShaders();
	map = TileMap::createTileMap("levels/prueba.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
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

	vector<glm::vec2> posEnemies;
	posEnemies.push_back(glm::vec2(22, 11)); posEnemies.push_back(glm::vec2(40, 11));
	posEnemies.push_back(glm::vec2(51, 11)); posEnemies.push_back(glm::vec2(52, 11));
	posEnemies.push_back(glm::vec2(80,  3)); posEnemies.push_back(glm::vec2(82,  3));
	posEnemies.push_back(glm::vec2(97, 11)); posEnemies.push_back(glm::vec2(99, 11));
	posEnemies.push_back(glm::vec2(114, 11)); posEnemies.push_back(glm::vec2(116, 11));
	posEnemies.push_back(glm::vec2(124, 11)); posEnemies.push_back(glm::vec2(126, 11));
	posEnemies.push_back(glm::vec2(129, 11)); posEnemies.push_back(glm::vec2(131, 11));
	posEnemies.push_back(glm::vec2(174, 11)); posEnemies.push_back(glm::vec2(176, 11));

	/* Goombas */
	for (int i = 0; i < 16; ++i) {
		Goomba* e = new Goomba;
		e->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		e->setPosition(glm::vec2(posEnemies[i].x * map->getBlockSize(), posEnemies[i].y * map->getBlockSize()));
		e->setOriginalPosition(glm::vec2(posEnemies[i].x * map->getBlockSize(), posEnemies[i].y * map->getBlockSize()));
		e->setTileMap(map);
		enemies.push_back(e);
	}

	posEnemies.push_back(glm::vec2(107, 11)); posEnemies.push_back(glm::vec2(14, 10)); // Koopas
	/* Koopas */
	for (int i = 16; i < 18; ++i) {
		Koopa* k = new Koopa;
		k->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		k->setPosition(glm::vec2(posEnemies[i].x * map->getBlockSize(), posEnemies[i].y * map->getBlockSize()));
		k->setOriginalPosition(glm::vec2(posEnemies[i].x * map->getBlockSize(), posEnemies[i].y * map->getBlockSize()));
		k->setTileMap(map);
		enemies.push_back(k);
	}

	pause = false, keyPausePressed = false;
	playingMusic = false; gameOver = false;

	if (!text.init("fonts/super-mario-bros-nes.ttf")) {
		cout << "Could not load font!!!" << endl;
	}
}

void Scene::restart() {
	currentTime = 0.0f;

	/* --- Player --- */
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getBlockSize(), INIT_PLAYER_Y_TILES * map->getBlockSize()));

	/* --- Camera --- */
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));

	/* --- Map --- */
	map->restart();

	/* --- Score --- */
	Score::instance().decreaseLive();
	Score::instance().restart();

	/* --- Enemies --- */
	for (Enemy* e : enemies) {
		e->restart();
	}

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
	map = TileMap::createTileMap("levels/prueba.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getBlockSize(), INIT_PLAYER_Y_TILES * map->getBlockSize()));
	player->setTileMap(map);
	camera = Projection(glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1)));
	currentTime = 0.0f;
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

	/* --- Time --- */
	currentTime += deltaTime;

	/* --- Map --- */
	map->update(deltaTime);
	if (!player->isDead()) map->collisionWithItems(player);

	/* --- Camera --- */
	float actualMid = camera.getXmid();

	/* --- Player --- */
	player->update(deltaTime, camera.getXmin(), actualMid, sound);

	/* --- Enemies (Update & Collision with Player) --- */
	for (auto* enemy : enemies) {
		enemy->update(deltaTime, camera.getXmin(), actualMid);

		if (player->isDead()) continue;
		int col = enemy->collision(player->getPos(), player->getSize());

		if (player->isStarMario() && col != 0) {
			cout << "Star Mario Collision" << endl;
			enemy->kill();
			continue;
		}
		if (col > 0) {
			// Mario Kills Enemy, needs to jump
			player->jumpEnemy();
		}
		else if (col < 0 && !player->isImmune()) {
			// Enemy kills Mario
			player->collisionEnemy();
		}
	}

	/* Enemies (Collision with other Enemies) */
	for (int i = 0; i < enemies.size(); ++i) {
		//if () // Poner un if para saltar más rápido
		for (int j = i + 1; j < enemies.size(); ++j) {
			int col = enemies[i]->collision(enemies[j]->getPos(), enemies[j]->getSize());

			if (col < 0) {
				bool e1 = enemies[i]->canKillEnemies();
				bool e2 = enemies[j]->canKillEnemies();

				// Passive Collision: We need to change directions of both of them
				if (!e1 && !e2) {
					enemies[i]->changeDirection();
					enemies[j]->changeDirection();
				}
				// Kill Collision: enemies[i] kills the enemies[j]
				else if (e1 && !e2) {
					enemies[j]->kill();
				}
				// Kill Collision: enemies[j] kills the enemies[i]
				else if (!e1 && e2) {
					enemies[i]->kill();
				}
				// Mutual Kill Collision: both enemies die
				else {
					enemies[i]->kill();
					enemies[j]->kill();
				}
			}
		}
	}

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
	map->render(camera.getPosition(), camera.getSize());

	/* Enemies */
	for (auto* enemy : enemies) {
		enemy->render();
	}

	/* Player */
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