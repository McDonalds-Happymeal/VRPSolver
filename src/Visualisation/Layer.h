#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GLhelper.h"

//wrapper for index buffer object storing additional draw information such as color and scale.
struct DrawProp {
	DrawProp(unsigned int* data, unsigned int count, unsigned int _drawmode, Color _color, float _scale);
	~DrawProp();

	IndexBuffer indexbuffer;
	unsigned int count;
	unsigned int drawmode;
	Color color;
	float scale;
};

//wrapper for vertex buffer and array. takes DrawRpop as input for drawing.
class Layer {
private:
	Shader* shader;
	VertexBuffer vertexBuffer;
	VertexArray vertexArray;
public:
	Layer(Shader* _shader,float* data, unsigned int size);
	~Layer();

	void draw(DrawProp* i);
};

