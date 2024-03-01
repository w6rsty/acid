#pragma once

#include "renderer/renderer_command.hpp"
#include "renderer/renderer3d.hpp"

namespace acid
{

class Renderer
{
public:
    static void Init()
    {  
        RendererCommand::Init();
        Renderer3D::Init();
    }

    static void Shutdown()
    {
        Renderer3D::Shutdown();
    }
};

} // namespace acid