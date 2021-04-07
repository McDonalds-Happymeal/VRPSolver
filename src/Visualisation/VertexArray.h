#pragma once
#include "VertexBuffer.h"
#include "VertexBufferAttributes.h"
#include "GLhelper.h"

//creates and handles Vertex arrays.

class VertexArray
{
private:
	unsigned int RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vertexBuffer, VertexBufferAttributes& layout);

	void Bind();
	void Unbind();
};

