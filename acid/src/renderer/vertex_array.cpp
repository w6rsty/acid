#include "renderer/vertex_array.hpp"

#include "core/assert.hpp"
#include "renderer/graphic_api.hpp"
#include "platform/OpenGL/opengl_vertex_array.hpp"

namespace acid
{

Ref<VertexArray> VertexArray::Create()
{ 
    switch (AcidGraphicAPI)
    {
        case AcidGraphicAPI::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case AcidGraphicAPI::OpenGL: return CreateRef<OpenGLVertexArray>();
    }
}

} // namespace acid