#include <iostream>
#include <cmath>
#include "Scene.h"


Scene::Scene()
{
	for(int i = 0; i < 4; i++)
		quads[i] = NULL;
}

Scene::~Scene()
{
	for(int i = 0; i < 4; i++)
		if(quads[i] != NULL)
			delete quads[i];
}


void Scene::init()
{
	initShaders();
	for(int i = 0; i < 4; i++)
		quads[i] = Quad::createQuad(-0.75f + (i % 2), -0.75f + (i / 2), 0.5f, 0.5f, program);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Scene::render(float scale, int quadStart, bool colStop) {
	// Change quad colors using elapsed time
	float value;
	
	if (colStop) value = 0.0f;
	else value = (sin(currentTime / 1000.f) + 1.0f) / 2.0f;

	program.use();
	program.setUniform2f("scale", scale, scale);
	program.setUniform4f("color", value, value, value, 1.0f);
	quads[quadStart]->render();
	program.setUniform4f("color", 1.0f, value, value, 1.0f);
	quads[(quadStart + 1) % 4]->render();
	program.setUniform4f("color", value, 1.0f, value, 1.0f);
	quads[(quadStart + 2) % 4]->render();
	program.setUniform4f("color", value, value, 1.0f, 1.0f);
	quads[(quadStart + 3) % 4]->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if(!program.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	program.bindFragmentOutput("outColor");
}

