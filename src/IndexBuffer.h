#pragma once
class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* bufferData, unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();
	
	unsigned int GetCount();
};

