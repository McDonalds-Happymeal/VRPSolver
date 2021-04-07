#include "Window.h"

Window::Window(Renderer* _renderer, int _moniter) : renderer(_renderer), moniter(_moniter)
{
	if (moniter < 1) {
		std::cout << "No Moniter selected running in window mode" << std::endl;
		scale = 0.5f;
	}
	else scale = 1.0f;
}

Window::Window(Renderer* _renderer, float _scale) : renderer(_renderer), scale(_scale), moniter(-1)
{
	if (scale > 1.0f) {
		std::cout << "window scales higher then 1.0 may be larger then current screen resolution!" << std::endl;
	}
}

Window::~Window()
{
	std::cout << "Window object Destroyed!!!" << std::endl;
}

int Window::CreateWindow(const char* title)
{

	if (!glfwInit()) {
		std::cout << "glfw failed to initialise!!!" << std::endl;
		return -1;
	}

	int moniterCount;
	GLFWmonitor** moniters = glfwGetMonitors(&moniterCount);

	if (moniterCount == NULL) {
		std::cout << "NO MONITORS PRESENT!!!" << std::endl;
		return -1;
	}

	//set open GL versions and profiles.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	/*code here could do with work!!*/
	/* Create a windowed mode window and its OpenGL context */
	if (0 < moniter) {
		if (moniter > moniterCount) {
			std::cout << "selected moniter does not exist!!!" << std::endl;
			return -1;
		}

		//creates full screen moniter with 1:1 aspect ratio.
		const GLFWvidmode* mode = glfwGetVideoMode(moniters[moniter-1]);
		window = glfwCreateWindow(mode->width, mode->height, title, moniters[moniter-1], NULL);
	}
	else {
		//creates window based on resolution of primary moniter.
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int resolution = static_cast<int>((mode->width > mode->height ? mode->height : mode->width) * scale);
		window = glfwCreateWindow(resolution, resolution, title, NULL, NULL);
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
	return 0;
}

void Window::Run()
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

	//return 0;
}
