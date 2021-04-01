#include "Renderer.h"

Renderer::Renderer(int points[], unsigned int distributionID, Color _bgColor)
{

}

/*
void Renderer::draw(VertexArray* va, IndexBuffer* ib, Shader* shader)
{
    shader->Bind();
    //shader->SetUniform4f("u_Color", r, g, b, 1.0f);

    va->Bind();
    ib->Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}
*/
void Renderer::Clear()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
