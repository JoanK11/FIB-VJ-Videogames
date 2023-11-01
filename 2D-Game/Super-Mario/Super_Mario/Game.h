#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Scene.h"

// la resolución original era 256*240 (512x480). En nuestro caso debería ser mínimo el doble en cada lugar
#define SCREEN_WIDTH  832
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game {

private:
	Game() {}
	
public:
	static Game &instance()	{
		static Game G;
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	bool getModifierKey(int key) const;
	void clearInput();

private:
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that we can have access at any time
	bool modifierKeys[3]; // 0 = shift, 1 = ctrl, 2 = alt
};


#endif // _GAME_INCLUDE


