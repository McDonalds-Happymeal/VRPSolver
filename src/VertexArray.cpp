#include "VertexArray.h"

//creates and handles Vertex arrays.

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &RendererID));
}

//builds and binds vertex buffer attributes and connects to vertexbuffer.
void VertexArray::AddBuffer(VertexBuffer& vertexBuffer, VertexBufferAttributes& layout)
{
	Bind();
	vertexBuffer.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size();i++) {
		auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalised, layout.GetStride(), (const void*)offset));//binds buffer to VAO
		offset += element.count * SizeofGLType(element.type);
	}
	
}

void VertexArray::Bind()
{
	GLCall(glBindVertexArray(RendererID));
}

void VertexArray::Unbind()
{
	GLCall(glBindVertexArray(0));
}

