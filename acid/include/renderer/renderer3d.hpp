#pragma once

#include "renderer/camera/scene_camera.hpp"

namespace acid
{

namespace geo 
{
    struct GeoData;
}

/// Benchmark
// 10000    cubes | 4096 | 210fps   | 20 drawcalls
// 10000    cubes | 8192 | 355fps   | 10 drawcalls

class Renderer3D
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Ref<SceneCamera>& camera);
    static void EndScene();
    static void Flush();

    static void DrawQuad(const glm::vec4& color = glm::vec4(1.0f), const glm::mat4& transform = glm::mat4(1.0f));
    static void DrawTriangle(const glm::vec4& color = glm::vec4(1.0f), const glm::mat4& transform = glm::mat4(1.0f));
    static void DrawCuboid(const glm::vec4& color = glm::vec4(1.0f), const glm::mat4& transform = glm::mat4(1.0f));
    static void DrawPyramid(const glm::vec4& color = glm::vec4(1.0f), const glm::mat4& transform = glm::mat4(1.0f));
    // avoid using it
    static void DrawCircle(const glm::vec4& color = glm::vec4(1.0f), const glm::mat4& transform = glm::mat4(1.0f));

    static const RendererStats& GetStats();
private:
    static void DrawSimpleGeo(const geo::GeoData& drawInfo, const glm::vec4& color, const glm::mat4& transform);

    static void StartBatch();
    static void NextBatch();
};

} // namespace acid