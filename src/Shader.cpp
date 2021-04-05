#include "Shader.h"

//all methods for loading parsing and creating a openGL program composing of a vertex and fragment shader.
//unifroms also handeled withen class.

//reads file and returns content to string.
std::string Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss;

    while (getline(stream, line)) {
        ss << line << '\n';
    }

    return ss.str();
}

//creates program by compiling given vertex and fragment shader code.
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDetachShader(program, vs));
    GLCall(glDetachShader(program, fs));
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

//complies shader code and checks for errors.
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    //ERROR HANDELER FOR FAILURE TO COMPILE.
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

//initilise object by creating program from shader code files.
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexShader = ParseShader(vertexPath);
    std::string fragmentShader = ParseShader(fragmentPath);

    std::cout << "VERTEX SHADER CODE: " << std::endl << vertexShader;
    std::cout << "FRAGMENT SHADER CODE: " << std::endl << fragmentShader;

    RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(RendererID));
}

void Shader::Bind() {
    GLCall(glUseProgram(RendererID));
}

void Shader::Unbind() {
    GLCall(glUseProgram(0));
}

//allows setting of uniform only supports setUnifrom4f as only colour values need to be supported for application.
void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4) {
    GLCall(glUniform4f(glGetUniformLocation(RendererID, name.c_str()),v1,v2,v3,v4))
}