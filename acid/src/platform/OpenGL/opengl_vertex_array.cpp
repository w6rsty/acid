#include "platform/OpenGL/opengl_vertex_array.hpp"

#include "glad/glad.h"

namespace acid
{

OpenGLVertexArray::OpenGLVertexArray()
{
    glCreateVertexArrays(1, &rendererID_);
    glBindVertexArray(rendererID_);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &rendererID_);
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(rendererID_);
}

void OpenGLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

} // namespace acid