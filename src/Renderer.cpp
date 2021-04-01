#include "Renderer.h"

void GLClearError() {
    while (glGetError());
}

bool GLCheckError(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

//find size of open GL types from GL common Enum. 
unsigned int SizeofGLType(unsigned int type)
{
    switch (type) {
        case GL_BYTE: return(sizeof(GLbyte));
        case GL_UNSIGNED_BYTE: return(sizeof(GLubyte));
        case GL_SHORT: return(sizeof(GLshort));
        case GL_UNSIGNED_SHORT: return(sizeof(GLushort));
        case GL_INT: return (sizeof(GLint));
        case GL_UNSIGNED_INT: return (sizeof(GLuint));
        case GL_FLOAT: return (sizeof(GLfloat));
        case GL_HALF_FLOAT: return (sizeof(GLhalf));
        case GL_DOUBLE: return (sizeof(GLdouble));
    }
    ASSERT(false);
    return 0;
}
