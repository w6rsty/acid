#pragma once

#include "core/base.hpp"
#include "renderer/buffer.hpp"
#include "renderer/camera/camera.hpp"
#include "window/window.hpp"
#include "renderer/camera/scene_camera.hpp"
#include "scene/scene.hpp"

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
    void Run();
private:
    bool running_ = true;

    // debug
    glm::vec3 cameraPos_ = {0.0f, 0.0f, 0.0f};
    glm::vec3 cameraRot_ = {0.0f, 0.0f, 0.0f};
    ProjectionType projectionType_ = ProjectionType::Perspective;
    int projectionTypeIndex_ = 0;

    Ref<Scene> scene_;
    Ref<FrameBuffer> frameBuffer_;

    friend class Application;
};

} // namespace acid