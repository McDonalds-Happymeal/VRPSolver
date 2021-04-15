#pragma once
#include <iostream>
#include <GL/glew.h>


//helper functions and macros for writing openGL code common to many files.
#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();x;ASSERT(GLCheckError(#x,__FILE__,__LINE__));

void GLClearError();

bool GLCheckError(const char* function, const char* file, int line);
#else
#define GLCall(x) x;
#endif // DEBUG

//returns size in bytes of openGL.
unsigned int SizeofGLType(unsigned int type);

//Stores rgba values in single struct.
struct Color {
	float r;
	float g;
	float b;
	float a;
};
