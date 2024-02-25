#pragma once

#include "window/window.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/shader.hpp"
#include "renderer/camera/scene_camera.hpp"

namespace acid
{

class Engine
{
public:
    Engine();
    ~Engine();

    void Init();

    void OnAttach();
    void OnDetach();

    void Run();
private:
    bool running_ = true;
    Ref<VertexArray> vertexArray_;
    Ref<Window> window_;
    Ref<Shader> shader_;
    Ref<SceneCamera> camera_;
};

} // namespace acid