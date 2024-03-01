#pragma once

#include "renderer/camera/scene_camera.hpp"
#include "renderer/texture.hpp"

namespace acid
{

namespace geo 
{
    struct GeoData;
}

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
    static const RendererStats& GetStats();
private:
    static void StartBatch();
    static void NextBatch();
};

} // namespace acid