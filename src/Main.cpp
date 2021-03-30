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
        char* infoLog = new char[length];
        glGetShaderInfoLog(id, length, &length, infoLog);
        std::cout << "Shader complie failure: " << infoLog << std::endl;

        glDeleteShader(id);
        delete[] infoLog;
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

    float points[8] = {
       -p,-p,
        p,-p,
        p, p,
       -p, p,
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    VertexBuffer *vertexBuffer = new VertexBuffer(points, 2 * 4 * sizeof(float));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));//binds buffer to VAO

    IndexBuffer *indexBuffer = new IndexBuffer(indices, 6);

    std::string vertexShader = ParseShader("resources/shaders/BasicVertex.Shader");
    std::string fragmentShader = ParseShader("resources/shaders/BasicFragment.Shader");

    std::cout << "VERTEX" << std::endl;
    std::cout << vertexShader << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << fragmentShader << std::endl;

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.2f, 0.8f, 0.8f, 1.0f));

    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = 0.0f, g = 0.5f, b = 1.0f;
    float increment[3] = { 0.05f, 0.051f, 0.052f };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, g, b, 1.0f));

        GLCall(glBindVertexArray(vao));
        indexBuffer->Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        neon(&r, &increment[0]);
        neon(&g, &increment[1]);
        neon(&b, &increment[2]);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);

    delete indexBuffer;
    delete vertexBuffer;

    glfwTerminate();
    return 0;
}