#include "renderer/texture.hpp"

#include "core/assert.hpp"
#include "renderer/renderer_api.hpp"
#include "platform/OpenGL/opengl_texture.hpp"
#include "renderer/renderer_enum.hpp"

namespace acid
{

// Ref<Texture> Shader::Create(const std::string& filename)
// {
//     switch (RendererAPI::GetAPI())
//     {
//         case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
//         case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(filename);
//     }
// }

Ref<Texture> Create(uint32_t width, uint32_t height, AC_TEXTURE_WARP_MODE mode)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height, mode);
    }
}

Ref<Texture> Create(const std::string& path, AC_TEXTURE_WARP_MODE mode)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path, mode);
    }
}

} // namespace acid