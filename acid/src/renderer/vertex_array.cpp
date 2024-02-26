#include "renderer/vertex_array.hpp"

#include "core/assert.hpp"
#include "renderer/renderer_api.hpp"
#include "platform/OpenGL/opengl_vertex_array.hpp"

namespace acid
{

Ref<VertexArray> VertexArray::Create()
{ 
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
    }
}

} // namespace acid