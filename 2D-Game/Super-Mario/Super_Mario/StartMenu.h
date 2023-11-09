#ifndef _STARTMENU_INCLUDE
#define _STARTMENU_INCLUDE

#include "Sprite.h"
#include "Text.h"
#include "TileMap.h"
#include "ShaderProgram.h"
#include "SoundManager.h"

class StartMenu {

public:
	void init();
	void update(int deltaTime);
	void render();

	void initShaders();
	void restartShaders();
	void openMenu();
	bool showingMenu() const;
	void updateValues();

private:
	bool visible;
	int pos;
	int posXini, posYini, espacio;
	Texture spritesheet;
	Sprite *sprite, * spriteWall;
	Text text, textKeyboard;
	glm::ivec2 posPlayer;
	ShaderProgram texProgram;
	SoundManager sound;

	bool keyUpPressed, keyDownPressed;
	bool keyEnterPressed, keyEPressed;
	int state;
};

#endif // _STARTMENU_INCLUDE