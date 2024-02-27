#pragma once

#include "renderer/camera/scene_camera.hpp"

namespace acid
{

namespace geo 
{
    struct GeoData;
}

/// Pure static 
class Renderer3D
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Ref<SceneCamera>& camera);
    static void EndScene();

    static void DrawQuad(const glm::mat4& transform = glm::mat4(1.0f));
    static void DrawTriangle(const glm::mat4& transform = glm::mat4(1.0f));
    static void DrawCircle(const glm::mat4& transform = glm::mat4(1.0f));
    static void DrawCuboid(const glm::mat4& transform = glm::mat4(1.0f));
    static void DrawPyramid(const glm::mat4& transform = glm::mat4(1.0f));
private:
    static void DrawSimpleGeo(const geo::GeoData& drawInfo, const glm::mat4& transform);
};

} // namespace acid