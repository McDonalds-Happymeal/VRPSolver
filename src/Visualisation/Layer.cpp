#include "Layer.h"

Layer::Layer(Shader* _shader, float* data, unsigned int size) :
	vertexBuffer(VertexBuffer(data, size * sizeof(float)))
{
	shader = _shader;

	VertexBufferAttributes layout;
	layout.Push(GL_FLOAT, 2);

	vertexArray.AddBuffer(vertexBuffer, layout);
}

Layer::~Layer()
{
}

void Layer::draw(DrawProp* i)
{
	shader->Bind();
	shader->SetUniform4f("u_Color", i->color.r, i->color.g, i->color.b, i->color.a);
	glPointSize(i->scale);
	glLineWidth(i->scale);
	vertexArray.Bind();
	i->indexbuffer.Bind();

	GLCall(glDrawElements(i->drawmode, i->count, GL_UNSIGNED_INT, nullptr))

	vertexArray.Unbind();
}

DrawProp::DrawProp(unsigned int* data, unsigned int _count, unsigned int _drawmode, Color _color, float _scale) : indexbuffer(IndexBuffer(data, _count))
{
	count = _count;
	drawmode = _drawmode;
	color = _color;
	scale = _scale;
}

DrawProp::~DrawProp()
{

}
