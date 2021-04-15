#pragma once

#include <algorithm>
#include <vector>
#include <iostream>
#include <mutex>
#include <queue>

#include "GL/glew.h"

//GL related helpers
#include "shader.h"
#include "GLhelper.h"
#include "Layer.h"

//forward declaration.
struct LinesTemplate {
	std::vector<unsigned int> lines;
	Color color;
	float scale;
};

class RenderData;
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
	std::vector<std::unique_ptr<DrawProp>> pointsPropertiesList;
	std::vector<std::unique_ptr<DrawProp>> linesPropertiesList;

	std::shared_ptr<RenderData> renderData;

public:
	Renderer(std::vector<double> points, std::shared_ptr<RenderData> _renderData, Color bgC = { 1.0f,1.0f,1.0f,1.0f });
	~Renderer();

	void pointsProperties(std::vector<unsigned int>, Color color, float scale);
	void pointsProperties(unsigned int data, Color color, float scale);
	void pointsPropertiesClear();
	
	void Draw();
};


class RenderData {
	friend void Renderer::Draw();
private:
	std::queue<LinesTemplate> linesPropertiesList;
	std::mutex mutex;
public:

	
	RenderData();
	~RenderData();

	//empty vector will be inerpreted as a line clear call.
	void AddLine(std::vector<unsigned int> data, Color color, float scale);
	void LinesClear();
};

