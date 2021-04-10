#include "Window.h"

//title is for window title, scale sets window reolution as % of shortest dimension, moniter > 0 wil lfullscreen on selected moniter.
Window::Window(const char* title, float _scale, int _moniter) {
	if (_scale > 1.0f) {
		std::cout << "window scales higher then 1.0 may be larger then current screen resolution!" << std::endl;
	}

	if (!glfwInit()) throw std::exception("glfw failed to initialise!!!");

	int moniterCount;
	GLFWmonitor** moniters = glfwGetMonitors(&moniterCount);

	if (moniterCount == NULL) throw std::exception("No Moniter of display found!");

	//set open GL versions and profiles.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	if (_moniter > moniterCount) {
		std::cout << "selected moniter does not exsist, program will open in window mode." << std::endl;
		_moniter = 0;
	}
	if (0 < _moniter) {
		//creates full screen moniter with 1:1 aspect ratio.
		const GLFWvidmode* mode = glfwGetVideoMode(moniters[_moniter - 1]);
		window = glfwCreateWindow(mode->width, mode->height, title, moniters[_moniter - 1], NULL);
	}
	else {
		//creates window based on resolution of primary moniter.
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int resolution = static_cast<int>((mode->width > mode->height ? mode->height : mode->width) * _scale);
		window = glfwCreateWindow(resolution, resolution, title, NULL, NULL);
	}


	if (!window)
	{
		glfwTerminate();
		throw std::exception("window could not initialise");
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);//sets frame rate to moniters

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << glewGetErrorString(err) << std::endl;
		throw std::exception("ERROR INITIALIZING GLEW!");
	}

	std::cout << "openGL version: " << glGetString(GL_VERSION) << std::endl;
}


Window::~Window()
{
	std::cout << "Window object Destroyed!!!" << std::endl;
}

//main run loop, will not return until termniated.
void Window::Run(Renderer* renderer)
{
	//open GL some hardcoded things to make renders nicer.
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	//MAIN RENDERER LOOP HERE!!!!
	while (!glfwWindowShouldClose(window))
	{
		/*RENDER HERE*/
		renderer->Draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();
}
