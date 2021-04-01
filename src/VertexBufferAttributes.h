#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	GLboolean normalised;
};

class VertexBufferAttributes
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferAttributes();

	void Push(unsigned int type, unsigned int count, bool normalise = GL_FALSE);

	//const std::vector<VertexBufferElement> GetElements() const& { return elements; }
	const std::vector<VertexBufferElement> GetElements();

	unsigned int GetStride();
};

