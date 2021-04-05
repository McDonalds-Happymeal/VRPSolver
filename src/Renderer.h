#pragma once

#include <algorithm>
#include <vector>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

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
	void Draw();
public:
	Renderer(std::vector<double> points, Color pointsC = { 1.0f,0.0f,0.0f,1.0f }, Color bgC = { 0.0f,0.0f,0.0f,1.0f });
	~Renderer();

	void SetIndexColor(const unsigned int* index, Color c);

	int Run(std::string name,unsigned int moniter = NULL);
	void Completed();
	void AddCompleted();
};