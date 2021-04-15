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
	//glfw title, scale is % of shortest screen resolution, moniter values greater then 0 will fullscreen window on selected moniter if availible.
	Window(const char* title, float _scale = 0.8f, int _moniter = 0);
	~Window();

	//run function requires rendererpointer for draw call function.
	void Run(Renderer* renderer);
};

