#include "Renderer.h"


//blank constructor all initilisation is done in INIT function.
//not happy with this implmentation possible improvment?
//defaults red points and white background;
Renderer::Renderer(std::vector<double> points, Color pointsC, Color bgC)
{

	//INIT SHADER-------------------------------------------------------
	this->shader = new Shader("resources/shaders/BasicVertex.Shader", "resources/shaders/BasicFragment.Shader");

	//INIT POINTS LAYER WITH NORMALISED POINTS + BORDER
	double border = 0.9;//values 1 + % of minimum distance points and windows border.
	std::vector<float> vertex;
	//finds min max for normalisation

	double xmin = points[0];
	double ymin = points[1];
	double xmax = points[0];
	double ymax = points[1];

	for (std::vector<double>::iterator it = points.begin(); it != points.end(); it++) {
		xmin = (xmin > *it) ? *it : xmin;
		xmax = (xmax < *it) ? *it : xmax;
		if (it == points.end()) break;//incase !point.length%2 == 0
		it++;
		ymin = (ymin > *it) ? *it : ymin;
		ymax = (ymax < *it) ? *it : ymax;
	}

	//normlises all values in array to between -1 and 1 (/border val) and pushes to vertex float.
	for (std::vector<double>::iterator it = points.begin(); it != points.end(); it++) {
		vertex.push_back(static_cast<float>((((*it - xmin) / (xmax - xmin)) * 2 * border) - border));
		if (it == points.end()) break;//incase !point.length%2 == 0
		it++;
		vertex.push_back(static_cast<float>((((*it - ymin) / (ymax - ymin)) * 2 * border) - border));
	}

	this->points = new Layer2D(shader, &vertex[0], vertex.size(), pointsC);//makes points red by default.

	//INIT BACKGROUND
	float bgvertex[8] = { -1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f };
	unsigned int bgindex[6] = { 0,1,2,0,2,3 };

	this->background = new Layer2D(shader, bgvertex, 8, bgC);//white background
	this->background->SetDefaultIndexBuffer(bgindex, 6);
}

Renderer::~Renderer()
{
	//clear heap memory;
	delete shader;
	delete background;
	delete points;
}

void Renderer::SetIndexColor(std::vector<unsigned int>index, Color c)
{
	pointsColors.push_back(LayerIndex(&index[0], index.size(), GL_POINTS, c));
}

void Renderer::SetIndexColor(unsigned int index, Color c)
{
	pointsColors.push_back(LayerIndex(&index, 1, GL_POINTS, c));
}

void Renderer::ClearIndexColors()
{
	pointsColors.clear();
}

void Renderer::DrawLines(std::vector<unsigned int> index, Color c, bool connected)
{
	if (connected) edges.push_back(LayerIndex(&index[0], index.size(), GL_LINE_STRIP, c));
	else edges.push_back(LayerIndex(&index[0], index.size(), GL_LINES, c));
}

void Renderer::ClearLines()
{
}


void Renderer::Draw()
{
	
	background->drawIndex(GL_TRIANGLES);

	if (!edges.empty()) {
		for (std::vector<LayerIndex>::iterator layer = edges.begin(); layer != edges.end(); layer++) {
			points->drawIndex(layer->indexBuffer, layer->drawmode, layer->color);
		}
	}
	
	points->drawVertex(GL_POINTS);
	
	if (!pointsColors.empty()) {
		for (std::vector<LayerIndex>::iterator layer = pointsColors.begin(); layer != pointsColors.end(); layer++) {
			points->drawIndex(layer->indexBuffer, layer->drawmode, layer->color);
		}
	}
	
}

void Renderer::SetScale(unsigned int scale)
{
	glPointSize(scale * 3);
	glLineWidth(scale);
}

