#include <iostream>

#include "CBCI.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <typeinfo>
#include <string>
#include <assert.h> 
#include <fstream>
#include <sstream>
#include <memory>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int helloWorld(int argc, std::string* argv) {
	std::cout << "Hello World! is running" << std::endl;

	return 1;
}

static void neon(float* val, float* increment) {
    if (*val > 1.0f)
        *increment = -*increment;
    else if (*val < 0.0f)
        *increment = -*increment;

    *val += *increment;
}
    
int main(int argc, char** argv) {

    //INITIATING GLFW, GLEW , and contexts.
    GLFWwindow* window;
    GLFWmonitor* moniter;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    moniter= glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(moniter);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);//mode->height
    if (!window)
    {
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

    //DATA FOR GPU-------------------------------------------------------

    float p = 0.9f;

    float points[10] = {
       -p,-p,
        p,-p,
        p, p,
       -p, p,
       0.0f,0.0f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    VertexArray* vertexArray = new VertexArray();
    VertexBuffer* vertexBuffer = new VertexBuffer(points, 2 * 5 * sizeof(float));

    VertexBufferAttributes layout;
    layout.Push(GL_FLOAT,2);
    vertexArray->AddBuffer(*vertexBuffer, layout);

    IndexBuffer *indexBuffer = new IndexBuffer(indices, 6);

    Shader* shader = new Shader("resources/shaders/BasicVertex.Shader", "resources/shaders/BasicFragment.Shader");
    shader->Bind();

    shader->SetUniform4f("u_Color", 0.2f, 0.8f, 0.8f, 1.0f);

    float r = 0.0f, g = 0.5f, b = 1.0f;
    float increment[3] = { 0.03f, 0.05f, 0.07f };

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /*RENDER HERE*/
        renderer.Clear();
        shader->SetUniform4f("u_Color", r, g, b, 1.0f);

        renderer.draw(vertexArray, indexBuffer, shader);

        neon(&r, &increment[0]);
        neon(&g, &increment[1]);
        neon(&b, &increment[2]);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete shader;
    delete indexBuffer;
    delete vertexBuffer;
    delete vertexArray;

    glfwTerminate();
    return 0;
}