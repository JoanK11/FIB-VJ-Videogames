#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	quad = NULL;
}

Scene::~Scene()
{
	if(quad != NULL)
		delete quad;
}


void Scene::init()
{
	initShaders();
	quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, program);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Scene::render()
{
	glm::mat4 modelview;

	// We can now, using matrices, draw four quads at different screen locations
	// using a single Quad object.
	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	// First we need to translate the quad to the center of the quads (256, 176)
	// Then we rotate him and translate him back to his original position

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 48.f, 0.f)); // Original Position
	//modelview = glm::translate(modelview, glm::vec3(128.f + 64.f, 128.f + 64.f, 0.f)); // Translate to center (considering 64.f quad size)
	//modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	//modelview = glm::translate(modelview, glm::vec3(-128.f - 64.f, -128.f - 64.f, 0.f)); // Translate to original position
	modelview = glm::translate(modelview, glm::vec3(sin(currentTime / 1000.f) * 128.f, 0.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	float sinus = sin(currentTime / 1000.f);
	float scale = sinus > 0.f ? sinus * 0.5f : sinus * 0.25f;

	modelview = glm::translate(glm::mat4(1.f), glm::vec3(384.f, 48.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(scale + 0.5f));
	modelview = glm::translate(modelview, glm::vec3(sin(currentTime / 1000.f)*128.f, 0.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 304.f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(128.f + 64.f, -128.f + 64.f, 0.f));
	//modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	//modelview = glm::translate(modelview, glm::vec3(-128.f - 64.f, 128.f - 64.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(sin(currentTime / 1000.f) * 128.f, 0.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();


	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 304.f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(-128.f + 64.f, -128.f + 64.f, 0.f));
	//modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	//modelview = glm::translate(modelview, glm::vec3(128.f  - 64.f, 128.f - 64.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(sin(currentTime / 1000.f) * 128.f, 0.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();
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

