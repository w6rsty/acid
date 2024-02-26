#pragma once

#include "renderer/camera/scene_camera.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"

namespace acid
{

/// Pure static 
class Renderer
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Ref<SceneCamera>& camera);
    static void EndScene();

    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);
};

} // namespace acid