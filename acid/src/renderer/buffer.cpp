#include "renderer/buffer.hpp"

#include "core/assert.hpp"
#include "renderer/renderer_api.hpp"
#include "platform/OpenGL/opengl_buffer.hpp"

namespace acid
{

Ref<VertexBuffer> VertexBuffer::Create(size_t size)
{ 
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
    }
}
    
Ref<VertexBuffer> VertexBuffer::Create(void* vertices, size_t size)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }
}

Ref<IndexBuffer> IndexBuffer::Create(void* data, size_t size)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(data, size);
    }
}

Ref<IndexBuffer> IndexBuffer::Create(size_t size)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(size);
    }
}

Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
    }
}

Ref<UniformBuffer> UniformBuffer::Create(size_t size, uint32_t binding)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLUniformBuffer>(size, binding);
    }
}

Ref<UniformBuffer> UniformBuffer::Create(size_t size, uint32_t binding, uint32_t offset)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLUniformBuffer>(size, binding, offset);
    }
}

} // namespace acid
