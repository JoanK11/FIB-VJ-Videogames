#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Tile.h"
#include "Brick.h"
#include <unordered_map>
#include <utility>

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return blockSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	
private:
	bool loadLevel(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	Tile* getTile(string type, ShaderProgram& s, glm::vec2 tileC, glm::vec2 tileS, glm::vec2 textureC, glm::vec2 textureS, Texture* t);

private:
	unordered_map<string, pair<int,bool>> dicc;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	Tile **map;

};


#endif // _TILE_MAP_INCLUDE


