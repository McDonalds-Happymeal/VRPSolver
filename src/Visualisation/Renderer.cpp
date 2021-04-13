#include "Renderer.h"

Renderer::Renderer(std::vector<double> points, Color bgC) 
{
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

	this->points = new Layer(&shader, &vertex[0], sizeof(float) * vertex.size());
}

Renderer::~Renderer() {
	delete &shader;
	delete &points;

}

void Renderer::Draw() {
	background.draw(&backgroundProp);
}