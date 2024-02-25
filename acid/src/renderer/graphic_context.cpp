#include "renderer/graphic_context.hpp"

#include "core/assert.hpp"
#include "renderer/graphic_api.hpp"
#include "platform/OpenGL/opengl_graphic_context.hpp"

namespace acid
{

Ref<GraphicContext> GraphicContext::Create(GLFWwindow* handle)
{
    switch (AcidGraphicAPI)
    {
        case AcidGraphicAPI::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case AcidGraphicAPI::OpenGL: return CreateRef<OpenGLGraphicContext>(handle);
    }
}

} // namespace acid