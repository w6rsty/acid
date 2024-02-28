#include "platform/OpenGL/opengl_buffer.hpp"

#include "core/assert.hpp"
#include "renderer/buffer.hpp"

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

void OpenGLVertexBuffer::SetData(void* data, size_t size, size_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(size_t size)
: size_(size)
{
    glGenBuffers(1, &rendererID_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, size_t size)
: size_(size)
{
    glGenBuffers(1, &rendererID_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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

void OpenGLIndexBuffer::SetData(void* data, size_t size, size_t offset)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
: spec_(spec)
{
    glGenFramebuffers(1, &rendererID_);
    glBindFramebuffer(GL_FRAMEBUFFER, rendererID_);

    glGenTextures(1, &colorAttachment_);
    glBindTexture(GL_TEXTURE_2D, colorAttachment_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec_.Width, spec_.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment_, 0);

    glGenTextures(1, &depthAttachment_);
    glBindTexture(GL_TEXTURE_2D, depthAttachment_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, spec_.Width, spec_.Width, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment_, 0);

    AC_ASSERT_MSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    glDeleteBuffers(1, &rendererID_);
    glDeleteTextures(1, &colorAttachment_);
}

void OpenGLFrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, rendererID_);
    glViewport(0, 0, spec_.Width, spec_.Height);
}

void OpenGLFrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace acid