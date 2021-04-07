#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* bufferData, unsigned int size)
{
    GLCall(glGenBuffers(1, &rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, bufferData, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::Bind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::Unbind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
