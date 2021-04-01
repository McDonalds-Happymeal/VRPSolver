#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GLhelper.h"


class Renderer {
public:
	void draw(VertexArray* va,IndexBuffer* ib, Shader* shader);
	void Clear();
};