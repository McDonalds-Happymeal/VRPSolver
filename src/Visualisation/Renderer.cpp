#include "Renderer.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}


Renderer::Renderer(std::vector<double> points,std::shared_ptr<RenderData> _renderData, Color bgC)
{
	renderData = _renderData;
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);



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

void Renderer::pointsProperties(std::vector<unsigned int> data, Color color, float scale)
{
	pointsPropertiesList.push_back(std::make_unique<DrawProp>(&data[0], data.size(), GL_POINTS, color, scale));
}

void Renderer::pointsProperties(unsigned int data, Color color, float scale)
{
	pointsPropertiesList.push_back(std::make_unique<DrawProp>(&data, 1, GL_POINTS, color, scale));
}

void Renderer::pointsPropertiesClear()
{
	pointsPropertiesList.clear();
}



void Renderer::Draw() {
	
	//test if new data availible, and then attempts lock.
	if (!renderData->linesPropertiesList.empty()) {
		if(renderData->mutex.try_lock()) {
			//if lock made will update local line Properties list.
			while (!renderData->linesPropertiesList.empty()) {
				LinesTemplate &tmp = renderData->linesPropertiesList.front();
				//blank lines vector interpretted as a local line properties clear call else adds to local list and pops from shared data queue
				if (tmp.lines.empty()) linesPropertiesList.clear();
				else linesPropertiesList.push_back(std::make_unique<DrawProp>(&tmp.lines[0], tmp.lines.size(), GL_LINE_STRIP, tmp.color, tmp.scale));
				renderData->linesPropertiesList.pop();
			}
			renderData->mutex.unlock();
		}
	}
	

	background.draw(&backgroundProp);


	if (!linesPropertiesList.empty()) {
		for (std::vector<std::unique_ptr<DrawProp>>::iterator dp = linesPropertiesList.begin(); dp != linesPropertiesList.end(); ++dp) {
			points->draw((*dp).get());
		}
	}

	if (!pointsPropertiesList.empty()) {
		for (std::vector<std::unique_ptr<DrawProp>>::iterator dp = pointsPropertiesList.begin(); dp != pointsPropertiesList.end(); ++dp) {
			points->draw((*dp).get());
		}
	}
}

/*-------------------render data class--------------------*/

RenderData::RenderData() {
}

RenderData::~RenderData(){
}


void RenderData::AddLine(std::vector<unsigned int> data, Color color, float scale)
{
	mutex.lock();
	//linesPropertiesList.push_back(std::make_unique<DrawProp>(&data[0], data.size(), GL_LINE_STRIP, color, scale));
	linesPropertiesList.push({ data,color,scale });
	mutex.unlock();
}

void RenderData::LinesClear()
{
	mutex.lock();
	//blank vector input will call clear line properties in renderer.
	linesPropertiesList.push({ std::vector<unsigned int>{},{0,0,0,0},0});
	mutex.unlock();
}