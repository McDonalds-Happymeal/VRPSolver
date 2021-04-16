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

//Template struct stores data needed to make lineProp used in RenderData.
struct LinesTemplate {
	std::vector<unsigned int> lines;
	Color color;
	float scale;
};

//forward decleration or render data.
class RenderData;


//Renderer Stores and handels all data related to interfacing with openGL.
class Renderer {
private:
	//extremly basic shaders.
	Shader shader = Shader("resources/shaders/BasicVertex.Shader", "resources/shaders/BasicFragment.Shader");
	
	//Background data
	float bgvertex[8] = { -1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f };
	Layer background = Layer(&shader, bgvertex, 8);

	unsigned int bgindex[6] = { 0,1,2,0,2,3 };
	Color white{ 1.0f,1.0f,1.0f,1.0f };
	DrawProp backgroundProp = DrawProp(bgindex, 6, GL_TRIANGLES, { 0,0,0,1 }, 1);

	//Layer data for all points.
	Layer* points;

	//DrawProp vectors contain all properties needed to draw elements.
	std::vector<std::unique_ptr<DrawProp>> pointsPropertiesList;
	std::vector<std::unique_ptr<DrawProp>> linesPropertiesList;

	//shared pointer to RenderData object that allows line data from external thread to be passed into renderer.
	std::shared_ptr<RenderData> renderData;

public:
	//vector of doubles representing all vertexs, the renderData object used for passing in external data and background color.
	Renderer(std::vector<double> points, std::shared_ptr<RenderData> _renderData, Color bgC = { 1.0f,1.0f,1.0f,1.0f });
	~Renderer();

	//fucntions relating to setting draw properties for all vertex.
	void pointsProperties(std::vector<unsigned int>, Color color, float scale);
	void pointsProperties(unsigned int data, Color color, float scale);
	void pointsPropertiesClear();
	
	//draw function to be handed to main draw loop.
	void Draw();
};


class RenderData {
	//allows Renderer::draw fucntion to access all datamemebers.
	friend void Renderer::Draw();
private:
	//queue for LineTemplates ready for input into Renderer.
	std::queue<LinesTemplate> linesPropertiesList;
	std::mutex mutex;
public:
	RenderData();
	~RenderData();

	//empty vector will be inerpreted as a line clear call.
	void tryAddLineClear(std::vector<unsigned int> data, Color color, float scale);
	void AddLine(std::vector<unsigned int> data, Color color, float scale);
	void LinesClear();
};

