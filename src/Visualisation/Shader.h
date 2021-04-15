#pragma once
#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GLhelper.h"

//all methods for loading parsing and creating a openGL program composing of a vertex and fragment shader.
//uniforms also handeled withen class.

class Shader
{
private:
	unsigned int RendererID;
	
	//takes filepath and returns file content as string.
	static std::string ParseShader(const std::string& filepath);

	//complies shader code from string and creates openGL program.
	static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	//complies shader code and handeles compliation errors.
	static unsigned int CompileShader(unsigned int type, const std::string& source);


	//unsigned int GetUniform(const std::string& Uname);

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();


	void Bind();
	void Unbind();

	void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	int GetUniformLocation(const std::string& name);
};

