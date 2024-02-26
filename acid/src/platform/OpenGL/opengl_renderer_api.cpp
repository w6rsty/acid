#include "platform/OpenGL/opengl_renderer_api.hpp"

#include "glad/glad.h"

namespace acid
{

void OpenGLRendererAPI::Init() 
{
    glEnable(GL_DEPTH_TEST);
    // draw frame line
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void OpenGLRendererAPI::Shutdown()
{

}

void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray)
{
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexCount(), GL_UNSIGNED_INT, 0);
}

} // namespace acid