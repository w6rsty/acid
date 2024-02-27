#pragma once

#include "renderer/renderer_command.hpp"
#include "renderer/renderer2d.hpp"
#include "renderer/renderer3d.hpp"

namespace acid
{

class Renderer
{
public:
    static void Init()
    {  
        RendererCommand::Init();
        Renderer2D::Init();
        Renderer3D::Init();
    }

    static void Shutdown()
    {
        Renderer2D::Shutdown();
        Renderer3D::Shutdown();
    }
};

} // namespace acid