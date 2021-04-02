#pragma once

#include <GL/glew.h>

#include <vector>
#include <queue>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GLhelper.h"

//Specialised renderer for rendering in real time VRP solution calls.
//once flag bool completed is set true, renderer will show found solution.

/*
class Renderer {
private:
	Shader* shader;
	VertexArray* backgroundVA;
	VertexBuffer* backgroundVB;
	IndexBuffer backgroundIB;

	VertexArray points;
	IndexBuffer distributionIndex;
	bool completed = false;

	std::vector<IndexBuffer> completedRoutes;
	//IndexBuffer tmpDraw;
	//std::queue<edge> tmpDrawQueue;

	void DrawBG();
	void DrawVertexPoints(VertexArray* va, Color c);
	void DrawIndexPoints(VertexArray* va, Color c);
	void DrawIndexLine(VertexArray* va, IndexBuffer* ib, Color c);
	void DrawTriangle(VertexArray* va, IndexBuffer* ib, Color c);


	void Drawcompleted();
public:
	Color bgColor = ColorWhite;
	Color pointsColor = ColorRed;
	Color distributionColor = ColorBlack;
	unsigned int scale = 5;
	unsigned int distributionScale = 7;

	Renderer(int points[],unsigned int distributionID, Color _bgColor = ColorWhite);
	~Renderer();
	void draw();
	void Clear();
};
*/