#pragma once
#include "VertexBuffer.h"
#include "VertexBufferAttributes.h"

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

