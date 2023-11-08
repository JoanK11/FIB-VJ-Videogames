#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE



#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Tile.h"
#include "Brick.h"
#include <unordered_map>
#include "IntBox.h"
#include <vector>
#include "Flag.h"
// forward declaration (.h) + circular inclusion (.cpp) for resolve the problem of circular dependency
class Coin;
class Object;
class Star;
class Mushroom;
class Player;
class Enemy;
// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	struct Info{
		int tilePos;
		bool isWall;
		char object;
	};
public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render(glm::vec2 pos, glm::vec2 size, ShaderProgram& texProgram) const;
	void free();
	
	glm::ivec2 getMapSize();
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, int* posX) const;
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2&, int* posX) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	// this is for the player
	
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, int* posX, bool superMario) const;
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2&, int* posX, bool superMario) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool superMario);

	bool isInside(const glm::ivec2& pos, const glm::ivec2& size) const;
	int getBlockSize() const;
    void update(int dt);
	void collisionWithItems(Player *ply);
	void restart();
	bool reachFinishLine(const glm::ivec2& pos, const glm::ivec2& size, bool superMario );
	bool animationOfFlag(float dt);
	void updateEnemies(int deltaTime, Player* player, float xmin, float xmax);
	bool reachEntranceCastle(const glm::vec2& pos);

	bool colisionDown(const glm::ivec2& pos, const glm::ivec2& size)const;
private:
	bool loadLevel(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	Tile* getTile(string type, ShaderProgram& s, glm::vec2 tileC, glm::vec2 tileS,glm::vec2 tileMapDisplay, glm::vec2 textureC, glm::vec2 textureS, Texture* t, TileMap* map);
	Object* get_Object(char type, ShaderProgram& s, glm::vec2 tileC, glm::vec2 tileS, glm::vec2 tileMapDisplay, TileMap* map);

private:
	unordered_map<string, Info> dicc;
	int nTiles;
	glm::ivec2  mapSize, tilesheetSize;
	glm::vec2 position;
	int blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	Tile **mapBlocks;
	Tile** mapBackground;
	std::vector<Object *> items;
	Flag* flag;
	std::vector<Enemy*> enemies;
	int blockOfEntranceCaste;

};



#endif // _TILE_MAP_INCLUDE


