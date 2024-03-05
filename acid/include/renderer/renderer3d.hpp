#pragma once

#include "renderer/camera/scene_camera.hpp"
#include "renderer/texture.hpp"
#include "renderer/light/light.hpp"

namespace acid
{

class Renderer3D
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Ref<SceneCamera>& camera);
    static void EndScene();

    static void SetPointLight(const PointLight& light);
    static void SetSpotLight(const SpotLight& light);
    static void SetDirLight(const DirLight& light);

    static void DrawCuboid(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
    static void DrawCuboid(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tint = glm::vec4(1.0f));

    static const RendererStats& GetStats();
};

} // namespace acid