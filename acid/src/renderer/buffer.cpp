#include "renderer/buffer.hpp"

#include "core/assert.hpp"
#include "renderer/graphic_api.hpp"
#include "platform/OpenGL/opengl_buffer.hpp"

namespace acid
{

Ref<VertexBuffer> VertexBuffer::Create(size_t size)
{ 
    switch (AcidGraphicAPI)
    {
        case AcidGraphicAPI::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case AcidGraphicAPI::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
    }
}
    
Ref<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size)
{
    switch (AcidGraphicAPI)
    {
        case AcidGraphicAPI::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case AcidGraphicAPI::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* data, size_t count)
{
    switch (AcidGraphicAPI)
    {
        case AcidGraphicAPI::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case AcidGraphicAPI::OpenGL: return CreateRef<OpenGLIndexBuffer>(data, count);
    }
}

Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
{
    switch (AcidGraphicAPI)
    {
        case AcidGraphicAPI::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case AcidGraphicAPI::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
    }
}

} // namespace acid
