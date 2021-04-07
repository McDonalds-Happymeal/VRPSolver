#include "IndexBuffer.h"

//creates index buffer and stores index in rendereID count should be number of unsigned int data points.
IndexBuffer::IndexBuffer(const unsigned int* bufferData, unsigned int _count) :count(_count)
{
    GLCall(glGenBuffers(1, &rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint) , bufferData, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &rendererID));
}

//binds buffer for use by draw calls.
void IndexBuffer::Bind()
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::Unbind()
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//returns number of datapoints for use in draw functions.
GLuint IndexBuffer::GetCount()
{
    return count;
}
