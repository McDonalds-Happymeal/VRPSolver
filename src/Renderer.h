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


	bool completed;
public:
	Renderer();
	~Renderer();

	void Init(std::vector<double> points, Color pointsC = { 1.0f,0.0f,0.0f,1.0f }, Color bgC = { 1.0f,1.0f,1.0f,1.0f });

	void SetIndexColor(const unsigned int* index, Color c);

	void Completed();
	void AddCompleted();

	void Draw();
};