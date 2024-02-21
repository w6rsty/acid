#pragma once

#include "renderer/graphic_context.hpp"

struct GLFWwindow;

namespace acid
{

class OpenGLGraphicContext final : public GraphicContext
{
public:
    OpenGLGraphicContext(GLFWwindow* handle);

    virtual void Init() override;
    virtual void Swap() override;
private:
    GLFWwindow* handle_;
};

} // namespace acid