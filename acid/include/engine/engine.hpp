#pragma once

#include "renderer/buffer.hpp"
#include "renderer/camera/camera.hpp"
#include "window/window.hpp"
#include "renderer/camera/scene_camera.hpp"

#include "renderer/texture.hpp"

namespace acid
{

class Application;

/// Driven the core functions
class Engine
{
public:
    Engine();
    ~Engine();

    void Init();
    void Run();
private:
    bool running_ = true;

    Ref<SceneCamera> camera_;
    glm::vec3 cameraPos_;
    glm::vec3 cameraRot_;
    ProjectionType projectionType_ = ProjectionType::Perspective;
    int projectionTypeIndex_ = 0;
    Ref<SceneCamera> overviewCamera_;

    Ref<Texture> texture_;
    Ref<FrameBuffer> frameBuffer_;

    friend class Application;
};

} // namespace acid