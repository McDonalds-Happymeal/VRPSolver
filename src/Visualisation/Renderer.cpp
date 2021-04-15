#include "Renderer.h"

#ifdef _DEBUG
//open GL error handeler.
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
#endif // DEBUG


Renderer::Renderer(std::vector<double> points,std::shared_ptr<RenderData> _renderData, Color bgC)
{
	//Enables open GL error handeling if debug configurations.
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
#endif // DEBUG

	//initiates render datapointer.
	renderData = _renderData;

	//INIT POINTS LAYER WITH NORMALISED POINTS + BORDER
	double border = 0.9;//values 1 + % of minimum distance points and windows border.
	std::vector<float> vertex;//tmp vector stores float data used to created vertex buffer

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

	//creates main points layer with normilised values.
	this->points = new Layer(&shader, &vertex[0], sizeof(float) * vertex.size());
}

Renderer::~Renderer() {
	delete &shader;
	delete &points;
}

//adds points properties data via vector of indexes.
void Renderer::pointsProperties(std::vector<unsigned int> data, Color color, float scale)
{
	pointsPropertiesList.push_back(std::make_unique<DrawProp>(&data[0], data.size(), GL_POINTS, color, scale));
}

//modify a single points properties via index.
void Renderer::pointsProperties(unsigned int data, Color color, float scale)
{
	pointsPropertiesList.push_back(std::make_unique<DrawProp>(&data, 1, GL_POINTS, color, scale));
}

//clears all points properties.
void Renderer::pointsPropertiesClear()
{
	pointsPropertiesList.clear();
}


//RenderData class is a freind of Draw().
void Renderer::Draw() {
	
	//if new lines properties data is availible in in renderData queue the data will be added to local line properties list.
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
	
	//draw background
	background.draw(&backgroundProp);

	//draw all lines first from lines properties data,
	if (!linesPropertiesList.empty()) {
		for (std::vector<std::unique_ptr<DrawProp>>::iterator dp = linesPropertiesList.begin(); dp != linesPropertiesList.end(); ++dp) {
			points->draw((*dp).get());
		}
	}

	//draw all points from pointsproperties
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

//add new lineTemplate in thread safe manner.
void RenderData::AddLine(std::vector<unsigned int> data, Color color, float scale)
{
	mutex.lock();
	linesPropertiesList.push({ data,color,scale });
	mutex.unlock();
}

//adds blank line properties tmeplate that will be interpreted as a local line properties clear when reahced by draw.
void RenderData::LinesClear()
{
	mutex.lock();
	//blank vector input will call clear line properties in renderer.
	linesPropertiesList.push({ std::vector<unsigned int>{},{0,0,0,0},0});
	mutex.unlock();
}