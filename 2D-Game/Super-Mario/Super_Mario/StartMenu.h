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

	void initShaders();
	void restartShaders();
	void openMenu();
	bool showingMenu() const;

private:
	bool visible;
	int pos;
	int posYini, espacio;
	Texture spritesheet, spritesheet2;
	Sprite *sprite, * spriteWall, *sprite3;
	Text text, textKeyboard;
	glm::ivec2 posPlayer;
	ShaderProgram texProgram;

	bool keyUpPressed, keyDownPressed;
	bool keyEnterPressed, keyEscPressed;
	int state;
};

#endif // _STARTMENU_INCLUDE