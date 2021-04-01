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

struct Color {
	float r;
	float g;
	float b;
	float a;
};

Color ColorWhite = { 0.0f,0.0f,0.0f,1.0f };
Color ColorRed = { 1.0f,0.0f,0.0f,1.0f };
Color ColorGreen = { 0.0f,1.0f,0.0f,1.0f };
Color ColorBlue = { 0.0f,0.0f,1.0f,1.0f };
Color ColorBlack = { 1.0f,1.0f,1.0f,1.0f };

struct Edge {
	unsigned int x;
	unsigned int y;
};


class Renderer {
private:
	VertexArray *backgroundVA = new VertexArray();
	IndexBuffer backgroundIB;

	Shader shader;
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
	void draw();
	void Clear();
};