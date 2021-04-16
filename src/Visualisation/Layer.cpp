#include "Layer.h"

Layer::Layer(Shader* _shader, float* data, unsigned int size) :
	vertexBuffer(VertexBuffer(data, size))
{
	shader = _shader;

	//create vertex buffer attrib and puch data type and 2d coords layout.
	VertexBufferAttributes layout;
	layout.Push(GL_FLOAT, 2);

	vertexArray.AddBuffer(vertexBuffer, layout);
}

Layer::~Layer()
{
}

//draws vertex with passed drawproperties.
//not code be improved as rebinds vertex buffer everycall.
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
	i->indexbuffer.Unbind();
}

//creates draw properties object.
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
