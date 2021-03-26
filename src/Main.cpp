#include <iostream>

#include "CBCI.h"
#include "GLFW/glfw3.h"
#include <typeinfo>
#include <string>

using namespace CBCI;

int helloWorld(int argc, std::string* argv) {
	std::cout << "Hello World! is running" << std::endl;

	return 1;
}

int main(int argc, char** argv) {
	userinput mainLoop = userinput();
	mainLoop.addCommand(helloWorld, "hello");
	mainLoop.run();

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}