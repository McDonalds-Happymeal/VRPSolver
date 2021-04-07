#pragma once
#include <GL/glew.h>
#include <iostream>

#include "Shader.h"

//helper functions and macros for writing openGL code common to many files.

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();x;ASSERT(GLCheckError(#x,__FILE__,__LINE__));

void GLClearError();

bool GLCheckError(const char* function, const char* file, int line);

unsigned int SizeofGLType(unsigned int type);

//Stores rgba values in single struct.
struct Color {
	float r;
	float g;
	float b;
	float a;
};
