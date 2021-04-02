#include "Layer.h"

//count size of data / 2. for 2d array.
Layer2D::Layer2D(Shader* _shader, const float* data, unsigned int size, Color c)
{
	count = size;
	//sets up buffers with all data.
	this->shader = _shader;
	defaultColor = c;
	vertexBuffer = new VertexBuffer(data, 2*size*sizeof(float));
	VertexBufferAttributes layout;
	layout.Push(GL_FLOAT, 2);
	vertexArray = new VertexArray();
	vertexArray->AddBuffer(*vertexBuffer, layout);
}

Layer2D::~Layer2D()
{
	delete vertexBuffer;
	delete vertexArray;
	if (indexSet) delete indexBuffer;
	
}

//sets index buffer with values replacing old if present and setting index set flag.
//count should be number of values in indices.
void Layer2D::SetDefaultIndexBuffer(const unsigned int* indices, unsigned int count)
{
	if (indexSet) {
		delete indexBuffer;
		indexBuffer = new IndexBuffer(indices, count);
	}
	else {
		indexBuffer = new IndexBuffer(indices, count);
		indexSet = true;
	}
}

//shouldnt be used for completness sake.
void Layer2D::UnSetDefaultIndexBuffer()
{
	delete indexBuffer;
	indexSet = false;
}


void Layer2D::drawIndex(unsigned int drawmode, Color c)
{
	if (indexSet) {
		shader->Bind();
		shader->SetUniform4f("u_Color", c.r, c.g, c.b, c.a);
		vertexArray->Bind();
		indexBuffer->Bind();
		GLCall(glDrawElements(drawmode, indexBuffer->GetCount(),GL_UNSIGNED_INT, nullptr));
		vertexArray->Unbind();
	} 
	else {
		return;
	}
}

void Layer2D::drawIndex(unsigned int drawmode)
{
	if (indexSet) {
		shader->Bind();
		shader->SetUniform4f("u_Color", defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
		vertexArray->Bind();
		indexBuffer->Bind();
		GLCall(glDrawElements(drawmode, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
		vertexArray->Unbind();
	}
	else {
		return;
	}
}

void Layer2D::drawVertex(unsigned int drawmode, Color c)
{
	shader->Bind();
	shader->SetUniform4f("u_Color", c.r, c.g, c.b, c.a);
	vertexArray->Bind();
	GLCall(glDrawArrays(drawmode, 0, count));
	vertexArray->Unbind();
}

void Layer2D::drawVertex(unsigned int drawmode)
{
	shader->Bind();
	shader->SetUniform4f("u_Color", defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
	vertexArray->Bind();
	GLCall(glDrawArrays(drawmode, 0, count));
	vertexArray->Unbind();
}
