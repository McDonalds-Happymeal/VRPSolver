#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GLhelper.h"

class Layer2D
{
private:
	Shader* shader;
	VertexBuffer* vertexBuffer;
	VertexArray* vertexArray;
	IndexBuffer* indexBuffer;
	bool indexSet = false;
	unsigned int count;
public:
	Color defaultColor;

	//shader uses "u_color" for color values.
	Layer2D(Shader* _shader,const float* data, unsigned int size, Color c);
	~Layer2D();

	void SetDefaultIndexBuffer(const unsigned int* indices, unsigned int count);

	void UnSetDefaultIndexBuffer();


	void drawIndex(unsigned int drawmode, Color c);
	void drawIndex(unsigned int drawmode);
	void drawIndex(IndexBuffer* tmpIB,unsigned int drawmode, Color c);
	void drawIndex(IndexBuffer* tmpIB, unsigned int drawmode);

	void drawVertex(unsigned int drawmode, Color c);
	void drawVertex(unsigned int drawmode);

};

struct LayerIndex {
	IndexBuffer* indexBuffer;
	unsigned int drawmode;
	Color color;

	LayerIndex(const unsigned int* indices, unsigned int count, unsigned int _drawmode, Color _color);
};