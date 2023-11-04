#include <iostream>
#include <fstream>
#include <sstream>
#include "TileMap.h"
#include "Coin.h"
#include "Object.h"
#include "Mushroom.h"
#include "Star.h"
#include "Player.h"
#include "Goomba.h"
#include "Koopa.h"
using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}

void TileMap::update(int dt) {
	int s = items.size();
	for (int i = 0; i < s; ++i) {
		items[i]->update(dt);
	}
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			if (mapBlocks[j * mapSize.x + i] != nullptr)  mapBlocks[j * mapSize.x + i]->update(dt);
		}

	}
}
TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{	
	items = vector<Object*>(0, nullptr);
	ifstream fin;
	string line;
	stringstream sstream;
	fin.open("dicc/dicc.txt");
	if (!fin.is_open())
		return;
	getline(fin, line);
	sstream.str(line);
	int size;
	sstream >> size;
	sstream.clear();
	for (int i = 0; i < size; ++i) {
		string a;
		Info b;
		getline(fin, line);
		sstream.str(line);
		sstream >> a >> b.tilePos >> b.isWall >> b.object;
		sstream.clear();
		dicc[a] = b;

	}
	loadLevel(levelFile, minCoords,program);
}

TileMap::~TileMap()
{
	if(mapBlocks != NULL)
		delete mapBlocks;
	if (mapBackground != NULL)
		delete mapBackground;
}


void TileMap::render(glm::vec2 pos, glm::vec2 size) const
{
	

	glm::vec2 posOriginal = (pos-position) / float(blockSize);
	glm::vec2 posFinal = posOriginal + size / float(blockSize);

	
	for (int j = max((int)posOriginal.y,0); j <= min((int)posFinal.y,mapSize.y-1); j++) {
		for (int i = max((int)posOriginal.x, 0); i <= min((int)posFinal.x,mapSize.x-1); i++) {
			if (mapBackground[j * mapSize.x + i] != nullptr)  mapBackground[j * mapSize.x + i]->render();
		}
	}
	flag->render();
	int s = items.size();
	for (int i = 0; i < s; ++i) {
		items[i]->render();
	}
	for (int j = max((int)posOriginal.y, 0); j <= min((int)posFinal.y, mapSize.y - 1); j++) {
		for (int i = max((int)posOriginal.x, 0); i <= min((int)posFinal.x, mapSize.x - 1); i++) {
			if (mapBlocks[j * mapSize.x + i] != nullptr)  mapBlocks[j * mapSize.x + i]->render();
		}
	}
	for (auto* enemy : enemies) {
		enemy->render();
	}
}

void TileMap::free()
{
	
}
void TileMap::restart() {
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			if (mapBlocks[j * mapSize.x + i] != nullptr) mapBlocks[j * mapSize.x + i]->restart();
		}
	}
	for (Object* i : items) {
		i->restart();
	}
	items = vector<Object*>(0, nullptr);
	flag->restart();
	for (Enemy* e : enemies) {
		e->restart();
	}
}
bool TileMap::loadLevel(const string &levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	position = minCoords;
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	//miramos si la primera fila pone TILEMAP
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;

	//seguidamente miramos las medidas del tilemap
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	sstream.clear();
	//miramos tama o de bloque logico
	getline(fin, line);
	sstream.str(line);
	sstream >> blockSize;
	sstream.clear();
	//miramos path field
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	//miramos el tama o de la textura
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	mapBackground = new Tile*[mapSize.x * mapSize.y];
	mapBlocks = new Tile * [mapSize.x * mapSize.y];
	glm::vec2 halfTexel;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());



	for(int j=0; j<mapSize.y; j++)
	{
		getline(fin, line);
		sstream.str(line);
		for(int i=0; i<mapSize.x; i++)
		{
			//sacamos la string
			string type;
			sstream >> type;
			//calculamos
			glm::vec2 pos = glm::vec2(i * blockSize, j * blockSize);
			Tile* ptr = getTile(type, program, pos, glm::vec2(blockSize, blockSize), minCoords,tilesheetSize, tileTexSize, &tilesheet, this);

			mapBackground[j * mapSize.x + i] = nullptr;
			mapBlocks[j * mapSize.x + i] = nullptr;
			if (dicc[type].isWall) mapBlocks[j * mapSize.x + i] = ptr;
			else mapBackground[j * mapSize.x + i] = ptr;
			
			++nTiles;
		}
		sstream.clear();
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	//Object* p = new Star(glm::vec2{ 8 * 32,12 * 32 }, glm::vec2{ 32,32 }, minCoords, this, &program, 2.0);
	//Object* p = new Mushroom(glm::vec2{ 8 * 32,12 * 32 }, glm::vec2{ 32,32 }, minCoords, this, &program, 2.0);
	//Object* p = new Coin(glm::vec2{ 8 * 32,11 * 32 },  glm::vec2{ 32,32 }, minCoords, this, &program);
	//items.push_back(p);
	getline(fin, line);
	sstream.str(line);
	int ymin, ymax, xlim;
	sstream >> ymin >> ymax >> xlim;
	sstream.clear();
	flag = new Flag(ymin * blockSize, ymax * blockSize, xlim * blockSize, minCoords, &program);

	//enemies
	getline(fin, line);
	sstream.str(line);
	int numberOfEnemies;
	sstream >> numberOfEnemies;
	sstream.clear();
	enemies = vector<Enemy*>(numberOfEnemies, nullptr);


	for (int i = 0; i < numberOfEnemies; ++i) {
		getline(fin, line);
		sstream.str(line);
		char type;
		int x, y;
		sstream >> type >> x >> y;
		sstream.clear();
		if (type == 'G') {
			Goomba* e = new Goomba;
			e->init(minCoords, program);
			e->setPosition(glm::vec2(x * this->getBlockSize(),y * this->getBlockSize()));
			e->setOriginalPosition(glm::vec2(x * this->getBlockSize(), y * this->getBlockSize()));
			e->setTileMap(this);
			enemies[i] = e;
		}
		else if (type == 'K') {
			Koopa* e = new Koopa;
			e->init(minCoords, program);
			e->setPosition(glm::vec2(x * this->getBlockSize(), y * this->getBlockSize()));
			e->setOriginalPosition(glm::vec2(x * this->getBlockSize(), y * this->getBlockSize()));
			e->setTileMap(this);
			enemies[i] = e;
		}
	}
	fin.close();
	

	return true;
}



// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.
bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, int* posX) const
{	
	if (pos.y >= mapSize.y * blockSize) return false;
	if (pos.x < 0.) return false;
	int x, y0, y1;

	x = pos.x / blockSize;
	y0 = pos.y / blockSize;
	y1 = (pos.y + size.y - 1) / blockSize;
	y0 = max(0, y0);
	y1 = min(y1, mapSize.y - 1);
	for (int y = y0; y <= y1; y++)
	{
		if (mapBlocks[y * mapSize.x + x] != nullptr && mapBlocks[y * mapSize.x + x]->isTouchable()) {
			if (*posX - blockSize * (x + 1) <= 4)
			{
				*posX = blockSize * (x + 1) + 2;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, int* posX) const
{	
	if (pos.y  >= mapSize.y * blockSize) return false;
	if (pos.x + size.x >= mapSize.x * blockSize) return false;
	int x, y0, y1;

	x = (pos.x + size.x) / blockSize;
	y0 = pos.y / blockSize;
	y1 = (pos.y + size.y - 1) / blockSize;
	y0 = max(0, y0);
	y1 = min(y1, mapSize.y - 1);
	for (int y = y0; y <= y1; y++)
	{
		if (mapBlocks[y * mapSize.x + x] != nullptr && mapBlocks[y * mapSize.x + x]->isTouchable()) {
			if (*posX - blockSize * x + size.x <= 4)
			{
				*posX = blockSize * x - size.x  -2 ;
				return true;
			}
		}

	}

	return false;
}

bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, int* posX, bool superMario) const {
	//SI NO PONGO ESTAS LINEAS, HAY ACCESOS A NULLPTR
	if (pos.y  >= mapSize.y * blockSize) return false;
	if (pos.x < 0.) return false;
	int x, y0, y1;

	x = (pos.x-1) / blockSize; // -1 para evitar que cambie entre STAND_LEFT y MOVE_LEFT
	if (superMario) { // para no hacer esto se tendria que hacer que pos.y entrase como pos.y - 32
		y0 = (pos.y - 31) / blockSize;
		y1 = (pos.y + size.y - 32 - 1) / blockSize;
	}
	else {
		y0 = pos.y / blockSize;
		y1 = (pos.y + size.y - 1) / blockSize;
	}
	y0 = max(0, y0);
	y1 = min(y1, mapSize.y - 1);
	//cout << "Left - x: " << x << " / y0: " << y0 << " / y1 : " << y1 << endl;
	for (int y = y0; y <= y1; y++) {
		if (mapBlocks[y * mapSize.x + x] != nullptr && mapBlocks[y * mapSize.x + x]->isTouchable()) {
			if (*posX - blockSize * (x + 1) <= 4) {
				*posX = blockSize * (x + 1) +2;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, int* posX, bool superMario) const {
	if (pos.y  >= mapSize.y * blockSize) return false;
	if (pos.x + size.x >= mapSize.x * blockSize) return false;

	int x, y0, y1;

	x = (pos.x + size.x) / blockSize;
	if (superMario) { // para no hacer esto se tendria que hacer que pos.y entrase como pos.y - 32
		y0 = (pos.y - 31) / blockSize;
		y1 = (pos.y + size.y -32 - 1) / blockSize;
	}
	else {
		y0 = pos.y / blockSize;
		y1 = (pos.y + size.y - 1) / blockSize;
	}
	y0 = max(0, y0);
	y1 = min(y1, mapSize.y - 1);
	//cout << "Right - x: " << x << " / y0: " << y0 << " / y1 : " << y1 << endl;
	for (int y = y0; y <= y1; y++) {
		if (mapBlocks[y * mapSize.x + x] != nullptr && mapBlocks[y * mapSize.x + x]->isTouchable()) {
			if (*posX - blockSize * x + size.x <= 4) {
				*posX = blockSize * x - size.x -2;
				return true;
			}
		}

	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const {
	if (pos.y + size.y >= mapSize.y * blockSize) return false;
	int x0, x1, y;

	x0 = pos.x / blockSize;
	x1 = (pos.x + size.x - 1) / blockSize;
	y = (pos.y + size.y - 1) / blockSize;
	for (int x = x0; x <= x1; x++)
	{
		if (mapBlocks[y * mapSize.x + x] != nullptr && mapBlocks[y * mapSize.x + x]->isTouchable())
		{
			if (*posY - blockSize * y + size.y <= 4)
			{
				*posY = blockSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool superMario){
	if (!isInside(pos, size)) return false;
	int x0, x1, y;

	x0 = (pos.x + size.x*0.3f) / blockSize;
	x1 = (pos.x + 0.7f * size.x - 1) / blockSize;
	if (superMario) y = (pos.y - 32) / blockSize; // Jeremy mira el valor correcto del 31 porfa
	else y  = pos.y / blockSize;

	cout << x0 << ", " << x1 << endl;
	for (int x = x0; x <= x1; x++) {
		if (mapBlocks[y * mapSize.x + x] != nullptr && mapBlocks[y * mapSize.x + x]->isTouchable()) {
			if (!superMario && *posY - blockSize * (y + 1) <= 4) {
				 // Jeremy mira el valor correcto del 2 porfa
				*posY = blockSize * (y + 1);
				Object* obj = mapBlocks[y * mapSize.x + x]->actionToTouch(superMario);
				if (obj != nullptr) items.push_back(obj);
				return true;
			}
			else if (superMario && *posY - blockSize * (y + 2) <= 4) {
			    *posY = blockSize * (y + 2); // Jeremy mira el valor correcto del 2 porfa
				Object* obj = mapBlocks[y * mapSize.x + x]->actionToTouch(superMario);
				if (obj != nullptr) items.push_back(obj);
				return true;
			}
		}
	}

	return false;
}



bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	if (!isInside(pos, size)) return false;
	int x0, x1, y;

	x0 = pos.x / blockSize;
	x1 = (pos.x + size.x - 1) / blockSize;
	y = (pos.y) / blockSize;
	for (int x = x0; x <= x1; x++)
	{
		if (mapBlocks[y * mapSize.x + x] != nullptr && mapBlocks[y * mapSize.x + x]->isTouchable())
		{
			if (*posY - blockSize * (y + 1) <= 4)
			{
				*posY = blockSize * (y + 1);
				return true;
			}
		}
	}

	return false;
}
Tile* TileMap::getTile(string type, ShaderProgram& s, glm::vec2 tileC, glm::vec2 tileS, glm::vec2 tileMapDisplay, glm::vec2 textureC, glm::vec2 textureS, Texture* t, TileMap* map) {
	//we select the info object
	Info obj = dicc[type];
	
	//we look if the tile has an object
	if (obj.object != 'N') {
		Object* p = get_Object(obj.object, s, tileC, tileS, tileMapDisplay, map);
		return new IntBox(tileC + tileMapDisplay, tileS, &s, p);
	}
	if (obj.tilePos == -1) return nullptr;
	glm::vec2 texturePos = glm::vec2(float(obj.tilePos % tilesheetSize.x) / tilesheetSize.x, float(obj.tilePos / tilesheetSize.x) / tilesheetSize.y);
	if(obj.isWall) return new Brick(tileC+tileMapDisplay, tileS, texturePos, textureS, s, t);
	else return new Tile(tileC + tileMapDisplay, tileS, texturePos, textureS, s, t);
}

Object* TileMap::get_Object(char type, ShaderProgram& s, glm::vec2 tileC, glm::vec2 tileS, glm::vec2 tileMapDisplay, TileMap* map) {
	if (type == 'C') {
		return new Coin(tileC, tileS, tileMapDisplay, map,&s);
	}
	else if (type == 'M') {
		return new Mushroom(tileC, tileS, tileMapDisplay, map, &s, 2.0);
	}
	else if (type == 'S') {
		return new Star(tileC, tileS, tileMapDisplay, map, &s, 2.0);
	}
	else if (type == 'E') {
		return nullptr;
	}
	return nullptr;
}

void TileMap::collisionWithItems(Player* ply) {
	int s = items.size();
	for (int i = 0; i < s; ++i) {
		//will have to pass a bool of size to ensure that we have good collision
		if (items[i]->collide(ply->getPos(), ply->getSize())) items[i]->actionOfObject(ply);
	}
}

bool TileMap::isInside(const glm::ivec2& pos, const glm::ivec2& size) const{
	int xmin = pos.x;
	int xmax = pos.x + size.x;
	int ymin = pos.y;
	int ymax = pos.y + size.y;

	bool x = xmin >= 0 && xmax < (mapSize.x) * blockSize;
	bool y = ymin >= 0 && ymax < (mapSize.y) * blockSize;
	return x && y;

}

int TileMap::getBlockSize() const {
	return blockSize;
}

glm::ivec2 TileMap::getMapSize() {
	return mapSize;
}

bool TileMap::reachFinishLine(const glm::ivec2& pos, const glm::ivec2& size, bool superMario) {
	return flag->touchTheFlag(pos, size, superMario);
}
bool TileMap::animationOfFlag(float dt) {
	return flag->update(dt);
}

void TileMap::updateEnemies(int deltaTime, Player* player, float xmin, float xmax) {
	/* --- Enemies (Update & Collision with Player) --- */
	for (auto* enemy : enemies) {
		enemy->update(deltaTime, xmin, xmax);

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
}