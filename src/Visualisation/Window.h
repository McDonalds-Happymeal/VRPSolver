#pragma once
#include <stdlib.h>
#include <iostream>
#include <exception>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"

//Window object is a container object for a glfw window context.
class Window
{
private:
	GLFWwindow* window;

public:
	Window(const char* title, float _scale = 0.8f, int _moniter = 0);
	~Window();

	void Run(Renderer* renderer);
};

