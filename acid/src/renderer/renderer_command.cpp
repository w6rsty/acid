#include "renderer/renderer_command.hpp"

#include "platform/OpenGL/opengl_renderer_api.hpp"

namespace acid
{

RendererAPI* RendererCommand::rendererAPI_ = new OpenGLRendererAPI();

} // namespace acid