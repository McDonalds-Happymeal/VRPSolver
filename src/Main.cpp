#include <iostream>

#include "CBCI.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <typeinfo>
#include <string>
#include <assert.h> 
#include <fstream>
#include <sstream>

static std::string ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss;

    while (getline(stream, line)) {
        ss << line << '\n';
    }
    
    return ss.str();
}

using namespace CBCI;

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int state;
    glGetShaderiv(id, GL_COMPILE_STATUS, &state);
    if (state == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));//allows stack allocation to avoid heap.
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Shader complie failure: " << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    assert(("vertex shader failure", vs));
    assert(("fragment shader failure", fs));


    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int helloWorld(int argc, std::string* argv) {
	std::cout << "Hello World! is running" << std::endl;

	return 1;
}

int main(int argc, char** argv) {

    GLFWwindow* window;

    /* Initialize glfw library */
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

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "ERROR INITIALIZING GLEW! :" << glewGetErrorString(err);
        return -1;
    }

    std::cout << "openGL version: " <<  glGetString(GL_VERSION) << std::endl;

    float points[6] = {
       -0.5f,-0.5f,
        0.0f, 0.5f,
        0.5f,-0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), points, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    std::string vertexShader = ParseShader("resources/shaders/BasicVertex.Shader");
    std::string fragmentShader = ParseShader("resources/shaders/BasicFragment.Shader");

    std::cout << "VERTEX" << std::endl;
    std::cout << vertexShader << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << fragmentShader << std::endl;

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0,3);
        //glDrawElements(GL_TRIANGLES, 3, );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}