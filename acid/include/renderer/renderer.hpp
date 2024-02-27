#pragma once

#include "renderer/camera/scene_camera.hpp"

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

    static void DrawCuboid(const glm::mat4& transform);
};

} // namespace acid