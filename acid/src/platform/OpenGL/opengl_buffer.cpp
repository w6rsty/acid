#include "platform/OpenGL/opengl_buffer.hpp"

#include "glad/glad.h"

namespace acid
{

OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
{
    glGenBuffers(1, &rendererID_);
    glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, size_t size)
{
    glGenBuffers(1, &rendererID_);
    glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}   

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &rendererID_);
}

void OpenGLVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, rendererID_);

}

void OpenGLVertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(void* data, size_t size)
{
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* data, size_t count)
: count_(count)
{
    glGenBuffers(1, &rendererID_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &rendererID_);
}

void OpenGLIndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
}

void OpenGLIndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
{

}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{

}

void OpenGLFrameBuffer::Bind() const
{

}

void OpenGLFrameBuffer::Unbind() const
{

}

} // namespace acid