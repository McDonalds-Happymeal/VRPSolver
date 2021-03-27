//vertex shader
#version 330

layout(location = 0) in vec4 position;
void main()
{
	gl_Position = vec4(position);
};
