#pragma once

#include "renderer/camera/scene_camera.hpp"

namespace acid
{

class Renderer2D
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Ref<SceneCamera>& camera);
    static void EndScene();

    static void DrawQuad(const glm::mat4& transform);
    static void DrawCircle(const glm::mat4& transform);
};

} // namespace acid