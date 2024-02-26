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

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* data, size_t count)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(data, count);
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

} // namespace acid
