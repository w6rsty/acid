#pragma once

#include "core/base.hpp"
#include "scene/scene.hpp"
#include "window/window.hpp"
#include "renderer/buffer.hpp"
#include "renderer/camera/scene_camera.hpp"

namespace acid
{

class Application;

/// Driven the core functions
class RenderEngine
{
public:
    RenderEngine();
    ~RenderEngine();

    void Init();
    void LoadScene(const Ref<Scene>& scene);
    void Run();
private:
    bool ready_ = true;
    bool running_ = true;

    Ref<Scene> currentScene_;

    Ref<FrameBuffer> frameBuffer_;

    // debug
    struct DebugCameraData
    {
        glm::vec3 Pos = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rot = {0.0f, 0.0f, 0.0f};
        ProjectionType ProjectionType = ProjectionType::Perspective;
        int ProjectionTypeIndex = 0;
    } cameraData_;

    friend class Application;
};

} // namespace acid