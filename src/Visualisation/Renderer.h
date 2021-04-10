#pragma once

#include <algorithm>
#include <vector>
#include <iostream>

#include "GL/glew.h"

//GL related helpers
#include "shader.h"
#include "GLhelper.h"
#include "Layer.h"



class Renderer {
private:
	Shader* shader;
	Layer2D* background;
	Layer2D* points;
	std::vector<LayerIndex> pointsColors;
	std::vector<LayerIndex> edges;
public:
	Renderer(std::vector<double> points, Color pointsC = { 1.0f,0.0f,0.0f,1.0f }, Color bgC = { 1.0f,1.0f,1.0f,1.0f });
	~Renderer();

	void SetIndexColor(std::vector<unsigned int>index, Color c);
	void SetIndexColor(unsigned int index, Color c);
	void ClearIndexColors();

	void DrawLines(std::vector<unsigned int> index, Color c = { 1.0f,1.0f,1.0f,1.0f }, bool connected = false);
	void ClearLines();

	void Draw();
	void SetScale(unsigned int scale);
};