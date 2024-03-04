#include "platform/OpenGL/opengl_renderer_api.hpp"

#include "glad/glad.h"

namespace acid
{

void OpenGLRendererAPI::Init() 
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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


void OpenGLRendererAPI::DrawWireFrame(bool enable)
{
    if (enable)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount)
{
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void OpenGLRendererAPI::DrawIndexedArray(const Ref<VertexArray> &vertexArray, uint32_t indexCount, uint32_t count)
{
    vertexArray->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, count);
}

} // namespace acid