#ifndef _STARTMENU_INCLUDE
#define _STARTMENU_INCLUDE

#include "Sprite.h"
#include "Text.h"
#include "TileMap.h"
#include "ShaderProgram.h"

class StartMenu {

public:
	void init();
	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2& pos);

	void initShaders();

	bool showingMenu() const;

private:
	bool visible;
	int pos;
	Texture spritesheet;
	Sprite *sprite;
	Text text;
	glm::ivec2 tileMapDispl, posPlayer;
	ShaderProgram texProgram;

	bool keyUpPressed, keyDownPressed;
	bool keyEnterPressed;
};

#endif // _STARTMENU_INCLUDE