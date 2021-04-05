#include "Renderer.h"

void Renderer::Draw()
{
	background->drawIndex(GL_TRIANGLES);
	points->drawVertex(GL_POINTS);
}

//defaults red points and white background;
Renderer::Renderer(std::vector<double> points, Color pointsC, Color bgC)
{

	//INIT SHADER-------------------------------------------------------
	this->shader = new Shader("resources/shaders/BasicVertex.Shader", "resources/shaders/BasicFragment.Shader");

	//INIT POINTS LAYER WITH NORMALISED POINTS + BORDER
	double border = 1.1;//values 1 + % of minimum distance points and windows border.
	//finds min max for normalisation
	double min = (*std::min_element(points.begin(), points.end()))* border;
	double max = (*std::max_element(points.begin(), points.end())) * border;
	
	std::vector<float> vertex;

	for(std::vector<double>::iterator it = points.begin();it != points.end();it++) {
		vertex.push_back(static_cast<float>(*it - min) / (max - min));//normlises all values in array to between -1 and 1 (/border val) and pushes to vertex float.
	}

	this->points =  new Layer2D(shader, &vertex[0], vertex.size(), pointsC);//makes points red by default.

	//INIT BACKGROUND
	float bgvertex[8] = { 0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f };
	unsigned int bgindex[6] = { 0,1,2,4,0,2 };

	this->background = new Layer2D(shader, bgvertex, 8, bgC);//white background
	this->background->SetDefaultIndexBuffer(bgindex, 6);

}



Renderer::~Renderer()
{
	delete shader;
	delete background;
	delete points;
}

//windowsed mode unless moniter settwill return 0 if succesful;
int Renderer::Run(std::string name, unsigned int fullscreenMoniter)
{
	int moniterCount;
	GLFWwindow* window;
	GLFWmonitor** moniters = glfwGetMonitors(&moniterCount);

	if (moniterCount == NULL) {
		std::cout << "NO MONITORS PRESENT!!!" << std::endl;
		return -1;
	}

	if (!glfwInit()) {
		std::cout << "glfw failed to initialise!!!" << std::endl;
		return -1;
	}

	//set open GL versions and profiles.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		

	/*code here could do with work!!*/
	/* Create a windowed mode window and its OpenGL context */
	if (!fullscreenMoniter) {
		if (fullscreenMoniter > moniterCount) {
			std::cout << "selected moniter does not exsist!!!" << std::endl;
			return -1;
		}

		//creates full screen moniter with 1:1 aspect ratio.
		const GLFWvidmode* mode = glfwGetVideoMode(moniters[fullscreenMoniter]);
		int resolution = mode->width > mode->height ? mode->height : mode->width;
		window = glfwCreateWindow(resolution, resolution, name.c_str(), moniters[fullscreenMoniter], NULL);
	}
	else {
		//creates windows that is half the resolution of main moniter.
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int resolution = (mode->width > mode->height ? mode->height : mode->width)/2;
		window = glfwCreateWindow(resolution, resolution, name.c_str(), NULL, NULL);
	}

	if (!window)
	{
		std::cout << "window could not initialise" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);//sets frame rate to moniters

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "ERROR INITIALIZING GLEW! :" << glewGetErrorString(err);
		return -1;
	}

	std::cout << "openGL version: " << glGetString(GL_VERSION) << std::endl;

	//open GL some hardcoded things to make renders nicer.
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	//change these so not hard coded
	glPointSize(20);
	glLineWidth(10);

	//MAIN RENDERER LOOP HERE!!!!
	while (!glfwWindowShouldClose(window))
	{
		/*RENDER HERE*/
		Draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

