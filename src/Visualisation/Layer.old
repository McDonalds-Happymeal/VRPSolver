#include "Layer.h"


//count = length of data. for 2d array.
Layer2D::Layer2D(Shader* _shader, const float* data, unsigned int size, Color c)
{
	count = size/2;//draw call uses two data points.
	//sets up buffers with all data.
	this->shader = _shader;
	defaultColor = c;
	vertexBuffer = new VertexBuffer(data, size*sizeof(float));
	VertexBufferAttributes layout;
	layout.Push(GL_FLOAT, 2);
	vertexArray = new VertexArray();
	vertexArray->AddBuffer(*vertexBuffer, layout);

	//sets temp index buffer to be overwritten so that memebr varible is always initilised.
	unsigned int tmp = { 0 };
	indexBuffer = new IndexBuffer(&tmp, 1);
}

Layer2D::~Layer2D()
{
	//cleans up heap;
	delete vertexBuffer;
	delete vertexArray;
	delete indexBuffer;
}

//sets index buffer with values replacing old if present and setting index set flag.
//count should be number of values in indices.
void Layer2D::SetDefaultIndexBuffer(const unsigned int* indices, unsigned int count)
{
	indexBuffer = new IndexBuffer(indices, count);
}

//draws current default index with input drawmode.
void Layer2D::drawIndex(unsigned int drawmode)
{
	shader->Bind();
	shader->SetUniform4f("u_Color", defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
	vertexArray->Bind();
	indexBuffer->Bind();
	GLCall(glDrawElements(drawmode, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
	vertexArray->Unbind();
}

//draws current default index with input drawmode and color.
void Layer2D::drawIndex(unsigned int drawmode, Color c)
{
	shader->Bind();
	shader->SetUniform4f("u_Color", c.r, c.g, c.b, c.a);
	vertexArray->Bind();
	indexBuffer->Bind();
	GLCall(glDrawElements(drawmode, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
	vertexArray->Unbind();
}

//draw a temporary index buffer with input drawmode.
void Layer2D::drawIndex(IndexBuffer* tmpIB, unsigned int drawmode)
{
	shader->Bind();
	shader->SetUniform4f("u_Color", defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
	vertexArray->Bind();
	tmpIB->Bind();
	GLCall(glDrawElements(drawmode, tmpIB->GetCount(), GL_UNSIGNED_INT, nullptr));
	vertexArray->Unbind();
}

//draw a temporary index buffer with input drawmode and color.
void Layer2D::drawIndex(IndexBuffer* tmpIB, unsigned int drawmode, Color c)
{

	shader->Bind();
	shader->SetUniform4f("u_Color", c.r, c.g, c.b, c.a);
	vertexArray->Bind();
	tmpIB->Bind();
	GLCall(glDrawElements(drawmode, tmpIB->GetCount(), GL_UNSIGNED_INT, nullptr));
	vertexArray->Unbind();

}

//will draw all Vertex with input drawmode and color.
void Layer2D::drawVertex(unsigned int drawmode, Color c)
{
	shader->Bind();
	shader->SetUniform4f("u_Color", c.r, c.g, c.b, c.a);
	vertexArray->Bind();
	GLCall(glDrawArrays(drawmode, 0, count));
	vertexArray->Unbind();
}

//will draw all Vertex with input drawmode.
void Layer2D::drawVertex(unsigned int drawmode)
{
	shader->Bind();
	shader->SetUniform4f("u_Color", defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
	vertexArray->Bind();
	GLCall(glDrawArrays(drawmode, 0, count));
	vertexArray->Unbind();
}


/*-----------------------------------------------------------------------*/
//index layer constructor.
LayerIndex::LayerIndex(const unsigned int* indices, unsigned int count, unsigned int _drawmode, Color _color) : drawmode(_drawmode) , color(_color) {
	indexBuffer = new IndexBuffer(indices, count);
}
