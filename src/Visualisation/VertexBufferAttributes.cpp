#include "VertexBufferAttributes.h"

VertexBufferAttributes::VertexBufferAttributes()
{
	stride = 0;
}

void VertexBufferAttributes::Push(unsigned int type, unsigned int count, bool normalise)
{
	elements.push_back({ type, count,normalise });
	stride += count * SizeofGLType(type);
}

const std::vector<VertexBufferElement> VertexBufferAttributes::GetElements()
{
	return elements;
}

unsigned int VertexBufferAttributes::GetStride()
{
	return stride;
}
