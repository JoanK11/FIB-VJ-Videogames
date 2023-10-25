#ifndef _SCORE_INCLUDE
#define _SCORE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <utility>
#include "TexturedQuad.h"
#include "Text.h"

using namespace std;

class Score {

public:
	void init();
	void update(int deltaTime);
	void render();

	void increaseScore(int x);
	void increaseCoins(int x);
	void updateWorld(int x, int y);
	void decreaseLive();
	

private:
	int score;
	int coins;
	pair<int, int> world;
	double time;
	int lives;
	Text text;
};

#endif // _SCORE_INCLUDE