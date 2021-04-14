#pragma once

#include <algorithm>
#include <vector>
#include <iostream>
#include <mutex>

#include "GL/glew.h"

//GL related helpers
#include "shader.h"
#include "GLhelper.h"
#include "Layer.h"


class Renderer {
private:
	Shader shader = Shader("resources/shaders/BasicVertex.Shader", "resources/shaders/BasicFragment.Shader");
	
	//Background data
	float bgvertex[8] = { -1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f };
	Layer background = Layer(&shader, bgvertex, 8);

	unsigned int bgindex[6] = { 0,1,2,0,2,3 };
	Color white{ 1.0f,1.0f,1.0f,1.0f };
	DrawProp backgroundProp = DrawProp(bgindex, 6, GL_TRIANGLES, white, 1);

	Layer* points;

	//mutex stored as pointer on heap to allow access when object passed as refrence.
	std::vector<std::unique_ptr<DrawProp>> pointsPropertiesList;
	std::mutex* pointsMutex = new std::mutex();

	std::vector<std::unique_ptr<DrawProp>> linesPropertiesList;
	std::mutex* linesMutex = new std::mutex();
public:
	Renderer(std::vector<double> points, Color bgC = { 1.0f,1.0f,1.0f,1.0f });
	~Renderer();
	
	void pointsProperties(std::vector<unsigned int>, Color color, float scale);
	void pointsProperties(unsigned int data, Color color, float scale);
	void pointsPropertiesClear();

	void AddLine(std::vector<unsigned int>, Color color, float scale);
	void LinesClear();
	
	void Draw();
};