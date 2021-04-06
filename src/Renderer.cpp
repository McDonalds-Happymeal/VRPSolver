#include "Renderer.h"



Renderer::Renderer() {
}

Renderer::~Renderer()
{
	delete shader;
	delete background;
	delete points;
}

//defaults red points and white background;
void Renderer::Init(std::vector<double> points, Color pointsC, Color bgC)
{

	//INIT SHADER-------------------------------------------------------
	this->shader = new Shader("resources/shaders/BasicVertex.Shader", "resources/shaders/BasicFragment.Shader");

	//INIT POINTS LAYER WITH NORMALISED POINTS + BORDER
	double border = 0.9;//values 1 + % of minimum distance points and windows border.
	//finds min max for normalisation
	double min = (*std::min_element(points.begin(), points.end()));
	double max = (*std::max_element(points.begin(), points.end()));
	
	std::vector<float> vertex;

	for(std::vector<double>::iterator it = points.begin();it != points.end();it++) {
		vertex.push_back(static_cast<float>(((2*border)*(*it - min) / (max - min)))-border);//normlises all values in array to between -1 and 1 (/border val) and pushes to vertex float.
	}

	this->points =  new Layer2D(shader, &vertex[0], vertex.size(), pointsC);//makes points red by default.

	//INIT BACKGROUND
	float bgvertex[8] = { -1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f };
	unsigned int bgindex[6] = { 0,1,2,0,2,3 };

	this->background = new Layer2D(shader, bgvertex, 8, bgC);//white background
	this->background->SetDefaultIndexBuffer(bgindex, 6);
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

