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
    static void Flush();

    static void DrawVoxel(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
    static void DrawSprite(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& color = glm::vec4(1.0f));
    static void DrawPointLight(const glm::vec3& position, PointLight& Light, uint32_t index);
    static void DrawSpotLight(const glm::vec3& position, SpotLight& Light);
    static void SetGlobalLight(const DirLight& light);
    static void SetGamma(float* gamma);
    static const RendererStats& GetStats();
private:
    static void StartBatch();
    static void NextBatch();

    static void SetPointLightUniforms(const PointLight& light, uint32_t index);
    static void SetSpotLightUniforms(const SpotLight& light);
    static void SetDirLightUniforms(const DirLight& light);
};

} // namespace acid