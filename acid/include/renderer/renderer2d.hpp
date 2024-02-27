#pragma once

#include "renderer/camera/scene_camera.hpp"

namespace acid
{


/// Optimized for 2D rendering
class Renderer2D
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Ref<SceneCamera>& camera);
    static void EndScene();

    static void DrawQuad(const glm::mat4& transform = glm::mat4(1.0f));
};

} // namespace acid