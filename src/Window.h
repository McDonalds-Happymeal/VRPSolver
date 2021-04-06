#pragma once
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"

class Window
{
private:
	GLFWwindow* window;
	Renderer* renderer;
public:
	int moniter;
	float scale;

	Window(Renderer* _renderer,int _moniter);
	Window(Renderer* _renderer, float _scale);
	~Window();

	int CreateWindow(const char* title);
	void Run();
};

