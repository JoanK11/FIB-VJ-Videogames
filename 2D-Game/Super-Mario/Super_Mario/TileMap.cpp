#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
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
		pair<int,bool> b;
		getline(fin, line);
		sstream.str(line);
		sstream >> a >> b.first >>b.second;
		sstream.clear();
		dicc[a] = b;

	}
	loadLevel(levelFile, minCoords,program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render(glm::vec2 pos, glm::vec2 size) const
{
	//aplicamos el desplazamiento a la camara
	//hablarlo con joan
	glm::vec2 posOriginal = (pos-position) / float(blockSize);
	glm::vec2 posFinal = posOriginal + size / float(blockSize);

	
	for (int j = max((int)posOriginal.y,0); j <= min((int)posFinal.y,mapSize.y-1); j++) {
		for (int i = max((int)posOriginal.x, 0); i <= min((int)posFinal.x,mapSize.x-1); i++) {
			if (map[j * mapSize.x + i] != nullptr)  map[j * mapSize.x + i]->render();
		}
	}
}

void TileMap::free()
{
	
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
	
	map = new Tile*[mapSize.x * mapSize.y];

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
			glm::vec2 pos = glm::vec2(i * blockSize, j * blockSize) + minCoords;
			Tile* ptr = getTile(type, program, pos, glm::vec2(blockSize, blockSize), tilesheetSize, tileTexSize, &tilesheet);

			map[j * mapSize.x + i] = ptr;
			++nTiles;
		}
		sstream.clear();
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}



// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int* posX) const
{
	int x, y0, y1;
	
	x = pos.x / blockSize;
	y0 = pos.y / blockSize;
	y1 = (pos.y + size.y - 1) / blockSize;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y * mapSize.x + x] != nullptr && map[y * mapSize.x + x]->isTouchable()) {
			if (*posX - blockSize * (x+1)  <= 0)
			{
				*posX = blockSize * (x + 1);
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int* posX) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x) / blockSize;
	y0 = pos.y / blockSize;
	y1 = (pos.y + size.y - 1) / blockSize;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y * mapSize.x + x] != nullptr && map[y * mapSize.x + x]->isTouchable()) {
			if (*posX - blockSize * x + size.x <= 1)
			{
				*posX = blockSize * x - size.x;
				return true;
			}
		}
			
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / blockSize;
	x1 = (pos.x + size.x - 1) / blockSize;
	y = (pos.y + size.y - 1) / blockSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y * mapSize.x + x] != nullptr && map[y * mapSize.x + x]->isTouchable())
		{
			if(*posY - blockSize * y + size.y <= 4)
			{
				*posY = blockSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / blockSize;
	x1 = (pos.x + size.x - 1) / blockSize;
	y = (pos.y) / blockSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != nullptr && map[y * mapSize.x + x]->isTouchable())
		{
			if (*posY - blockSize * (y+1) <= 4)
			{
				*posY = blockSize * (y + 1);
				return true;
			}
		}
	}

	return false;
}
Tile* TileMap::getTile(string type, ShaderProgram& s, glm::vec2 tileC, glm::vec2 tileS, glm::vec2 tileSheetSize,glm::vec2 textureS, Texture* t) {
	//seleccionamos el tipo de tile
	pair<int,bool> obj = dicc[type];
	if (obj.first == -1) return nullptr;
	//calculamos la posicion de la textura
	glm::vec2 texturePos = glm::vec2(float(obj.first % tilesheetSize.x) / tilesheetSize.x, float(obj.first / tilesheetSize.x) / tilesheetSize.y);
	if(obj.second) return new Brick(tileC, tileS, texturePos, textureS, s, t);
	else return new Tile(tileC, tileS, texturePos, textureS, s, t);
}